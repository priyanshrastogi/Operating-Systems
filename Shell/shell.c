/*
AUTHOR - PRIYANSH RASTOGI
SGG3 PROJECT 1 : SHELL
A SIMPLE SHELL FOR LINUX WITH HISTORY FEATURE

HOW TO USE:
1. USE MAKEFILE TO COMPILE
2. ./shell
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/types.h>

#define MAX_LENGTH 100

int argc;
char line[MAX_LENGTH];
char* argv[100];
char* history = NULL;
ssize_t readline;
size_t len = 0;
char his[100];
FILE *file;

int linecount() {			//To Count lines in history.txt
		int i = 0;
		int ch = 0;
		FILE *hisfile;
		hisfile  = fopen("history.txt","r");
		while(!feof(hisfile)) {
			ch	= fgetc(hisfile);
			if(ch=='\n') {
				i++;
			}
		}
		fclose(hisfile);
		return i;
}

void execute() {		//Execute Process
	char *token;
	token = strtok(line," ");			//Split tokens from Command Line.
	int i=0;
	while(token!=NULL){
		argv[i]=token;
		token = strtok(NULL," ");
		i++;
	}
	argv[i]=NULL;                     //Set the last element of arv[] NULL for execvp()
	pid_t pid= fork();              	//Fork child
   	if(pid==0) {               		//Child
		execvp(*argv,argv);
		perror("Process Could Not Execute");
	}
	else if(pid<0)
		perror("Child Process Failed.");

	else {
		wait(NULL);
	}
}

void executeBackground() {		//Execute Process
	char *token;
	token = strtok(line," ");			//Split tokens from Command Line.
	int i=0;
	while(token!=NULL){
		argv[i]=token;
		token = strtok(NULL," ");
		i++;
	}
	argv[i]=NULL;                     //Set the last element of arv[] NULL for execvp()
	pid_t pid= fork();              	//Fork child
   	if(pid==0) {               		//Child
		execvp(*argv,argv);
		perror("Process Could Not Execute");
	}
	else if(pid<0)
		perror("Child Process Failed.");

	else {
	}
}

void showhistory() {	//To Show History
	file  = fopen("history.txt","a+");
	int n = 1;
	int i = linecount();
	if(i==0)
		printf("No History Found.\n");
	if(i>10) {													//If history has more than 10 commands.
		int k = i-10;
		while((readline=getline(&history,&len,file))!=-1) {		//Read history.txt line by line.
			if(n>k)												//Print only 10 recent commands. Least Recent to Most Recent.
				printf("%d  %s",n,history);
			n++;
		}
	}
	else {
		while((readline=getline(&history,&len,file))!=-1) {
			printf("%d  %s",n,history);
			n++;
		}
	}
	fprintf(file,"%s\n",line);
	fclose(file);
}

int main(){
	while(1) {
		file  = fopen("history.txt","a+");
		if(file==NULL)
			perror("Cannot Save or Show History. Cannot Load History File.");

		printf("osh>> ");			//Shell Prompt


		if(!fgets(line, MAX_LENGTH, stdin))		//Read Command
			break;
		fflush(stdout);

		strcpy(his,line);						//Copy line string to his string. his will be used to save command in history.

		size_t length = strlen(line);
		if (line[length - 1] == '\n')		//Remove \n, which is the last character of entered command.
		    line[length - 1] = '\0';

		if(strcmp(line, "exit")==0)			//If command is exit. Break the loop.
		    break;

		else if(strcmp(line, "history")==0)	{		//If command is history. Show the History
			showhistory();
		}

		else if(strcmp(line, "!!")==0)	{		//If command is !!. Execute the most recent command.
			int i;
			int j = 0;
			i = linecount();
			if(i==0)
				printf("No Recent Command Found.\n");
			while((readline=getline(&history,&len,file))!=-1) {
				j++;
				if(j==i) {
					strcpy(line,history);
					printf("%s",line);
					strcpy(his,line);
					length = strlen(line);
					if (line[length - 1] == '\n')		//Remove \n, which is the last character of entered command.
		    			line[length - 1] = '\0';
		    		if (strcmp(line, "history")==0)
		    			showhistory();
		    		else {
		    			execute();
		    			fprintf(file,"%s",his);
						fclose(file);
					}
				}
			}
		}

		else if((line[0]=='!')) {					//If first char of command is !
			int i=1;
			int j,k;
			size_t linelen = strlen(line);
			line[0] = '\0';							//Remove ! from line
			for(k=1;k<linelen;k++) {
				line[k-1]=line[k];					//Shift chars
			}
			line[linelen-1]='\0';					//Make last char NULL
			j = atoi(line);							//Convert line to integer j
			if(j==0)
				printf("Invalid Command. Please enter !! or !(CommandNum) to execute recent commands.\n");		//If command is not of type !N, print error.
			while((readline=getline(&history,&len,file))!=-1) {
				if(i==j) {							//If current read line is same as j
					strcpy(line,history);
					printf("%s",line);
					strcpy(his,line);
					length = strlen(line);
					if (line[length - 1] == '\n')		//Remove \n, which is the last character of entered command.
		    			line[length - 1] = '\0';
		   			if (strcmp(line, "history")==0)		//If the most recent command is history. Show History.
		   				showhistory();
		   			else {								//Else execute the most recent command.
		   				execute();
		   				fprintf(file,"%s",his);			//Add this to history
						fclose(file);
					}
		   		}
		   		i++;
			}
		}

		else if(strstr(line, "&") != NULL) {
			size_t linelen = strlen(line);
			line[linelen-1] = '\0';
			executeBackground();
			fprintf(file,"%s",his);
			fclose(file);
		}

		else{
			execute();
			fprintf(file,"%s",his);
			fclose(file);
		}
	}
}
