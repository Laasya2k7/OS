#include<stdio.h>
#include<unistd.h>

int main(){

printf("Before exec\n");
execlp("ls", "ls", "-l", NULL);
perror("execlp failed");

return 1;
}

