int cool(void) {
	int i;
	int k;

	i = 3;
	k = 0;
	srand((unsigned)time(NULL));
	k = rand() % i;
	return k;
}

const char* x[] = {
	"10.0.0.102", "10.0.0.101", "10.0.0.105",
};

int autoscan(char *s) {
	if (running == 1) {
		AdmSend("[AUTOSCAN RUNNING!!!]\n");
		return;
	}
	pid_t pid;
	unsigned long secs;
	time_t startx=time(NULL);
	time_t my_time;
	time(&my_time);
	struct tm * time_real; 
	time_real = localtime (&my_time);
	struct timespec start, end;
	double start_sec, end_sec, elapsed_sec;
	clock_gettime(CLOCK_REALTIME, &start);
	char autoscanstr[50];

	char i1[128]= "",i2[128]= "",i3[128]= "";

	char *text = s;
	sscanf(text, "%s %s %s", &i1, &i2, &i3);
	int slp = atoi(i1);

	secs = atoi(i2);

	int maxchlds= atoi(i3);

	AdmSend("%s %s %s",i1 ,i2, i3);

	if (mfork() != 0) return;
	while(1){

		if(time(NULL) >= startx+secs * 60){
			running=0;
			time(&my_time);
			time_real = localtime(&my_time);
			AdmSend("%s\n[ WAITING OF THREADS ]\n",asctime(time_real));
			--childs;
			kill((pid_t) getppid(), SIGTERM);
			time(&my_time);
			time_real = localtime(&my_time);
			AdmSend("%s\n[ AUTOSCAN DONE ]\n",asctime(time_real));
			break;
		}
		int rndip;
		rndip=rand()%255;
		time(&my_time);
		time_real = localtime(&my_time);	

		AdmSend("%s\n[AUTOSCAN-STARTED]RANGE[ %d.*.*.* ]EVERY[ %s mins]FOR[ %s mins]THREADS[ %s ]\n",asctime(time_real),rndip,i1,i2,i3);
		sprintf(autoscanstr,"%d %s %s",rndip,i3,i1);
		scanner(autoscanstr);
		sleep(slp * 100);

	}					
	clock_gettime(CLOCK_REALTIME, &end);
	start_sec = start.tv_sec + start.tv_nsec/NANO_PER_SEC;
	end_sec = end.tv_sec + end.tv_nsec/NANO_PER_SEC;
	elapsed_sec = end_sec - start_sec;
	AdmSend("The operation took %.3f seconds\n", elapsed_sec);
	exit(0);
}




