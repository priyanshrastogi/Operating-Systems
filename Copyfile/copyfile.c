/*
AUTHOR - PRIYANSH RASTOGI
SGG 3 PROJECT 2: COPYFILE USING PIPE

HOW TO USE:
1. USE MAKEFILE TO COMPILE
2. ./copyfile sourcefile destinationfile
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/types.h>

#define BUFFER_SIZE 1024
#define READ_END 0
#define WRITE_END 1

int main(int argc, char* argv[]) {

	int fd[2];
	pid_t cpid;
	pipe(fd);
	char read_buffer[BUFFER_SIZE];
	char write_buffer[BUFFER_SIZE];
	ssize_t readline;
	FILE *sourcefile;
	FILE *targetfile;

	if(argc<3) {
		printf("Please Enter a command with Atleast 2 Parameters. i.e. ./copyfile sourcefile destinationfile\n");
		exit(1);
	}

	/*Open Source File and Create File*/
	sourcefile  = fopen(argv[1],"r");
	if(sourcefile==NULL) {
		perror("Location of Sourcefile is incorrect ");
		exit(1);
	}

	targetfile  = fopen(argv[2],"w");
	if(targetfile==NULL) {
		perror("Location of Destintionfile is incorrect ");
		exit(1);
	}

	if((cpid = fork()) == -1) {
		perror("fork");
		exit(1);
	}

	if(cpid == 0){
		/*In child process, close write end. Until pipe has something to read, read from pipe and write it in destination file*/
		close(fd[WRITE_END]);
		while (read(fd[READ_END], write_buffer, BUFFER_SIZE)) {
			fputs(write_buffer, targetfile);
		}
		close(fd[READ_END]);
	}

	else {
			/*In parent process, close read end. Read maximum 1024 characters at a time and write it in pipe*/
			close(fd[READ_END]);
			while ((readline = fread(read_buffer, 1, sizeof read_buffer, sourcefile)) > 0) {
				write(fd[WRITE_END], read_buffer, BUFFER_SIZE);
			}
			close(fd[WRITE_END]);
	}

	return 0;
}
