#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include <stdlib.h>

#define ONE_MB (1024 * 1024)

void *sysinfos() {
	signal(SIGALRM,(void (*)(int))kill_child);
	child_pid = fork();
	if (child_pid > 0) {
		wait(NULL);

	}
	else if (child_pid == 0){
		//////////////CPU
		FILE *fp = fopen("/proc/cpuinfo", "r");
		assert(fp != NULL);
		size_t n = 0;
		char *line = NULL;
		//////////////////MEM
		long num_procs;
		long page_size;
		long num_pages;
		long free_pages;
		long long  mem;
		long long  free_mem;
		num_procs = sysconf (_SC_NPROCESSORS_CONF);
		page_size = sysconf (_SC_PAGESIZE);
		num_pages = sysconf (_SC_PHYS_PAGES);
		free_pages = sysconf (_SC_AVPHYS_PAGES);
		mem = (long long) ((long long)num_pages * (long long)page_size);
		mem /= ONE_MB;
		free_mem = (long long)free_pages * (long long)page_size;
		free_mem /= ONE_MB;
		AdmSend("yaBoT Remote System Information\n");
		/////////////////////////IP
		struct ifaddrs * ifAddrStruct=NULL;
		void * tmpAddrPtr=NULL;

		getifaddrs(&ifAddrStruct);

		while (ifAddrStruct!=NULL) {
			if (ifAddrStruct->ifa_addr->sa_family==AF_INET) { // check it is IP4
				// is a valid IP4 Address
				tmpAddrPtr=&((struct sockaddr_in *)ifAddrStruct->ifa_addr)->sin_addr;
				char addressBuffer[INET_ADDRSTRLEN];
				inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);
				AdmSend("%s IP Address %s\n", ifAddrStruct->ifa_name, addressBuffer); 
				sleep(1);
			} else if (ifAddrStruct->ifa_addr->sa_family==AF_INET6) { // check it is IP6
				// is a valid IP6 Address
				tmpAddrPtr=&((struct sockaddr_in *)ifAddrStruct->ifa_addr)->sin_addr;
				char addressBuffer[INET6_ADDRSTRLEN];
				inet_ntop(AF_INET6, tmpAddrPtr, addressBuffer, INET6_ADDRSTRLEN);
				AdmSend("%s IP Address %s\n", ifAddrStruct->ifa_name, addressBuffer); 
				sleep(1);
			} 
			ifAddrStruct=ifAddrStruct->ifa_next;
		}



		//////////////////////////
		AdmSend("Total RAM %lld MB - Free RAM: %lld MB\n", mem, free_mem);
		while (getline(&line, &n, fp) > 1) {
			if (strstr(line, "model name") || strstr(line, "cpu cores")
					|| strstr(line, "cpu MHz")) {
				AdmSend("%s", line);
				sleep(1);
			}
		}
		free(line);
		fclose(fp);
		exit(0);
	}
}
