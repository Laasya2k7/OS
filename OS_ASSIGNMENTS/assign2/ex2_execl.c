#include <stdio.h>
#include <unistd.h>

int main(){
printf("Before exec\n");

execl("/bin/ls" , "ls" , "-l", NULL);

//This runs only if execl fails

perror("execl failed");
return 1;
}

