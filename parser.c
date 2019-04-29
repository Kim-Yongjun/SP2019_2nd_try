#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"

SYMBOL parse(int *waitpid, BOOLEAN makepipe, int *pipefdp){
	SYMBOL symbol, term;
	int argc, srcfd, dstfd;
	int pid, pipefd[2];
	char *argv[MAXARG + 1], srcfile[MAXFNAME];
	char dstfile[MAXFNAME];
	char word[MAXWORD];
	BOOLEAN append;

	argc = 0;
	srcfd = 0;
	dstfd = 1;

	while(TRUE){
		switch (symbol = getsymbol(word)){
			case S_WORD:
				if(argc == MAXARG){
					fprintf(stderr, "Too many args.\n");
					break;
				}

				argv[argc] = (char *)malloc(strlen(word) + 1);

				if(argv[argc] == NULL){
					fprintf(stderr, "Out of arg memory.\n");
					break;
				}

				strcpy(argv[argc], word);

				argc++;
				continue;

			case S_LT:
				if(makepipe){
					fprintf(stderr, "Extra <.\n");
					break;
				}

				srcfd = BADFD;
				continue;

			case S_GT:
			case S_GTGT:
				if(dstfd != 1){
					fprintf(stderr, "Extra > or >>.\n");
					break;
				}

				if(getsymbol(dstfile) != S_WORD){
					fprintf(stderr, "Illegal > or >>.\n");
					break;
				}

				dstfd = BADFD;
				append = (symbol == S_GTGT);
				continue;

			case S_BAR:
			case S_AMP:
			case S_SEMI:
			case S_NL:
				argv[argc] = NULL;
				if(symbol == S_BAR){
					if(dstfd != 1){
						fprintf(stderr, "> or >> conflicts with |.\n");
						break;
					}

					term = parse(waitpid, TRUE, &dstfd);
				}


				else{
					term = symbol;
				}
				
				if(makepipe){
					if(pipe(pipefd) == ERROR){
						syserr("pipe");
					}
	
					*pipefdp = pipefd[1];
					srcfd = pipefd[0];
				}

				pid = execute(argc, argv, srcfd, srcfile, dstfd, dstfile, append, term == S_AMP);

				if(symbol != S_BAR){
					*waitpid = pid;
				}

				if(argc == 0 && (symbol != S_NL || srcfd > 1)){
						fprintf(stderr, "Missing cmd.\n");
				}

				while(--argc >= 0){
					free(argv[argc]);
				}
				
				return term;

			case S_EOF:
				exit(0);
		}
	}
}



