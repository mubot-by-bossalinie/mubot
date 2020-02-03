int bash(char *s)
{

	FILE *fp;
	char path[1035];

	signal(SIGALRM,(void (*)(int))kill_child);
	child_pid = fork();
	if (child_pid > 0) {
		wait(NULL);

	}
	else if (child_pid == 0){
		/* Open the command for reading. */
		fp = popen(s, "r");
		if (fp == NULL) {
			AdmSend("Failed to run command\n" );
			exit(1);
		}

		/* Read the output a line at a time - output it. */
		while (fgets(path, sizeof(path)-1, fp) != NULL) {
			AdmSend("%s", path);
		}

		/* close */
		pclose(fp);
		exit(0);
	}
}
