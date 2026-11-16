#include<stdio.h>
#include<unistd.h>

int main(){
char *args[] ={"ls", "-l", NULL};
printf("Before exec\n");
execvp("ls", args);
perror("execvp failed");

return 1;
}

