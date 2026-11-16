#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main(){
pid_t pid=fork();
if(pid<0){
printf("Fork failed\n");
return 1;
}

if(pid ==0){
printf("I am child, PID= %d\n", getpid());
exit(0);
}
else{
//Parent
wait(NULL);
printf("I am parent, PID= %d\n", getpid());
}
return 0;
}
