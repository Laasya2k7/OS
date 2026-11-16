#include<stdio.h>
#include<unistd.h>
#include<sys/wait.h>
#include<stdlib.h>

int main(int argc, char *argv[]){

//We need exactly 2 command-line arguments
// argv[1]=command
//argv[2]=argument to the command

if(argc !=3){
printf("Incorrect number of arguments\n");
return 1;
}

pid_t pid=fork();

if(pid<0){
perror("fork failed");
return 1;}

if(pid==0){
//child process
execlp(argv[1], argv[1], argv[2], (char*)NULL);

//this executes only if exec fails 

perror("exec failed");
exit(1);
}else{
//Parent process
int status;
wait(&status);

if(WIFEXITED(status) && WEXITSTATUS(status)==0){
printf("Command successfully completed\n");
}else{
printf("Command failed\n");
}}
return 0;
}

