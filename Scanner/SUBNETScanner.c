void subneta(char *s)
{
	if (running == 1) {
		AdmSend("[SSHSCAN RUNNING!!!]");
		return;
	}
	char *temp;
	char *ip_ptr;
	int a,b,c,d;
	char ip[20];
	char *subnet = NULL;
	unsigned long secs;

	int maxchilds=100,status,pgid;
	int sub =0;
	pid_t pid;
	time_t my_time;
	time(&my_time);
	struct tm * time_real; 
	time_real = localtime (&my_time);
	struct timespec start, end;
	double start_sec, end_sec, elapsed_sec;
	clock_gettime(CLOCK_REALTIME, &start);

	char i1[128]= "", i2[128]= "";

	char *text = s;

	sscanf(text, "%s %s", &i1, &i2);



	subnet = malloc(strlen(i1) + 1);
	strcpy(subnet, i1);

	maxchilds=atoi(i2);




	running=1;

	time(&my_time);
	time_real = localtime(&my_time);
	AdmSend("%s\n[SUBNET-A-SCAN STARTED]SUBNET[ %s ]THREADS[ %s ]\n",asctime(time_real),i1,i2);
	if (mfork() == 0){
		for (a = 0; a <= 255; ++a) {
			if (childs >= maxchilds)
				wait(NULL);

			(void) sprintf(ip, "%s.%i", subnet, a);

			switch(fork()) {
				case 0:
#include "../BruteForce/Pass.c"
					break;
				case -1:
					exit(1);
				default:
					++childs;
					break;

			}

		}

		time(&my_time);
		time_real = localtime(&my_time);
		AdmSend("%s\n[ WAITING OF THREADS ]\n",asctime(time_real));
		while (--childs) wait(NULL);
		kill((pid_t) getppid(), SIGTERM);
		running=0;
		time(&my_time);
		time_real = localtime(&my_time);
		AdmSend("%s\n[ SCAN DONE ]\n",asctime(time_real));
		clock_gettime(CLOCK_REALTIME, &end);
		start_sec = start.tv_sec + start.tv_nsec/NANO_PER_SEC;
		end_sec = end.tv_sec + end.tv_nsec/NANO_PER_SEC;
		elapsed_sec = end_sec - start_sec;
		AdmSend("The operation took %.3f seconds\n", elapsed_sec);
	}





}



void subnetb(char *s)
{
	if (running == 1) {
		AdmSend("[SSHSCAN RUNNING!!!]\n");
		return;
	}
	char *temp;
	char *ip_ptr;
	int a,b,c,d;
	char ip[20];
	char *subnet = NULL;
	unsigned long secs;

	int maxchilds=100,status,pgid;
	int sub =0;
	pid_t pid;
	time_t my_time;
	time(&my_time);
	struct tm * time_real; 
	time_real = localtime (&my_time);
	struct timespec start, end;
	double start_sec, end_sec, elapsed_sec;
	clock_gettime(CLOCK_REALTIME, &start);

	char i1[128]= "", i2[128]= "";

	char *text = s;

	sscanf(text, "%s %s", &i1, &i2);



	subnet = malloc(strlen(i1) + 1);
	strcpy(subnet, i1);

	maxchilds=atoi(i2);




	running=1;
	time(&my_time);
	time_real = localtime(&my_time);
	AdmSend("%s\n[SUBNET-B-SCAN STARTED]SUBNET[ %s ]THREADS[ %s ]\n",asctime(time_real),i1,i2);
	if (mfork() == 0){
		for (a = 0; a <= 255; ++a) {
			for (b = 0; b <= 255; ++b) {
				if (childs >= maxchilds)
					wait(NULL);

				(void) sprintf(ip, "%s.%i.%i", subnet, a, b);

				switch(fork()) {
					case 0:
#include "../BruteForce/Pass.c"
						break;
					case -1:
						exit(1);
					default:
						++childs;
						break;
				}

			}

		}

		time(&my_time);
		time_real = localtime(&my_time);
		AdmSend("%s\n[ WAITING OF THREADS ]\n",asctime(time_real));
		while (--childs) wait(NULL);
		kill((pid_t) getppid(), SIGTERM);
		running=0;
		time(&my_time);
		time_real = localtime(&my_time);
		AdmSend("%s\n[ SCAN DONE ]\n",asctime(time_real));

		clock_gettime(CLOCK_REALTIME, &end);
		start_sec = start.tv_sec + start.tv_nsec/NANO_PER_SEC;
		end_sec = end.tv_sec + end.tv_nsec/NANO_PER_SEC;
		elapsed_sec = end_sec - start_sec;
		AdmSend("The operation took %.3f seconds\n", elapsed_sec);
	}





}





void subnetc(char *s)
{
	if (running == 1) {
		AdmSend("[SSHSCAN RUNNING!!!]\n");
		return;
	}
	char *temp;
	char *ip_ptr;
	int a,b,c,d;
	char ip[20];
	char *subnet = NULL;
	unsigned long secs;

	int maxchilds=100,status,pgid;
	int sub =0;
	pid_t pid;
	time_t my_time;
	time(&my_time);
	struct tm * time_real; 
	time_real = localtime (&my_time);
	struct timespec start, end;
	double start_sec, end_sec, elapsed_sec;
	clock_gettime(CLOCK_REALTIME, &start);

	char i1[128]= "", i2[128]= "";

	char *text = s;

	sscanf(text, "%s %s", &i1, &i2);



	subnet = malloc(strlen(i1) + 1);
	strcpy(subnet, i1);

	maxchilds=atoi(i2);




	running=1;
	time(&my_time);
	time_real = localtime(&my_time);
	AdmSend("%s\n[SUBNET-C-SCAN STARTED]SUBNET[ %s ]THREADS[ %s ]\n",asctime(time_real),i1,i2);
	if (mfork() == 0){
		for (a = 0; a <= 255; ++a) {
			for (b = 0; b <= 255; ++b) {
				for (c = 0; c <= 255; ++c) {
					if (childs >= maxchilds)
						wait(NULL);

					(void) sprintf(ip, "%s.%i.%i.%i", subnet, a, b, c);

					switch(fork()) {
						case 0:
#include "../BruteForce/Pass.c"
							break;
						case -1:
							exit(1);
						default:
							++childs;
							break;
					}

				}

			}
		}
		time(&my_time);
		time_real = localtime(&my_time);
		AdmSend("%s\n[ WAITING OF THREADS ]\n",asctime(time_real));
		while (--childs) wait(NULL);
		kill((pid_t) getppid(), SIGTERM);
		running=0;
		time(&my_time);
		time_real = localtime(&my_time);
		AdmSend("%s\n[ SCAN DONE ]\n",asctime(time_real));

		clock_gettime(CLOCK_REALTIME, &end);
		start_sec = start.tv_sec + start.tv_nsec/NANO_PER_SEC;
		end_sec = end.tv_sec + end.tv_nsec/NANO_PER_SEC;
		elapsed_sec = end_sec - start_sec;
		AdmSend("The operation took %.3f seconds\n", elapsed_sec);
	}





}
