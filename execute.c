#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "common.h"

int execute(int ac, char *av[], int *srcfd, char *srcfile,  int dstfd, char *dstfile, BOOLEAN append, BOOLEAN bg){
	int pid;

	if(ac == 0 || shellcmd(ac, av, srcfd, dstfd)){
		return 0;
	}

	pid = fork();

	switch (pid){
		case ERROR:
			fprintf(stderr, "Can't create new process.\n");
			return 0;

		case 0:
			redirect(srcfd, srcfile, dstfd, dstfile, append, bg);
			execvp(av[0], av);
			fprintf(stderr, "Can't execute %s\n", av[0]);
			exit(0);

		default:
			if(srcfd > 0 && close(srcfd) == ERROR){
				syserr("Close srcfd");
			}

			if(dstfd > 1 && close(dstfd) == ERROR){
				syserr("Close dstfd");
			}

			if(bg){
				printf("%d\n", pid);
			}

			return pid;
	}

}


