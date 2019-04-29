#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include "common.h"

void redirect(int srcfd, char *srcfile, int dstfd, char *dstfile, BOOLEAN append, BOOLEAN bg){
	int flags, fd;

	if(srcfd == 0 && bg){
		strcpy(srcfile, "/dev/null");
		srcfd = BADFD;
	}

	if(srcfd != 0){
		if(close(0) == ERROR){
			syserr("Close");
		}

		if(srcfd > 0){
			if(dup(srcfd) != 0){
				fatal("dup");
			}
		}

		else if(open(srcfile, O_RDONLY, 0) == ERROR){
			fprintf(stderr, "Can't open %s\n", srcfile);
			exit(0);
		}
	}

	if(dstfd != 1){
		if(close(1) == ERROR){
			syserr("Close");
		}

		if(dstfd > 1){
			if(dup(dstfd) != 1){
				fatal("dup");
			}
		}

		else{
			flags = O_WRONLY | O_CREAT;

			if(!append){
				flags |= O_TRUNC;
			}

			if(open(dstfile, flags, 0666) == ERROR){
				fprintf(stderr, "Can't create %s\n", dstfile);
				exit(0);
			}

			if(append){
				if(lseek(1, 0L, 2) == ERROR){
					syserr("lseek");
				}
			}
		}
	}


	for(fd = 3; fd < MAXFD; fd++){
		close(fd);
	}

	return;
}

