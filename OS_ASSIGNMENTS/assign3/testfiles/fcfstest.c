#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void
work(int id){
volatile int x=0;
for(int i=0;i<50000000;i++)x+=i;
printf("Process %d completed\n",id);}

int main(void){

int pid;

//Create process 1
pid=fork();

if(pid==0){
	work(1);
	exit(0);
}

//Delay before creating process 2

for(volatile int i=0;i<1000000;i++);

//Create process 2
pid=fork();

if(pid==0){
 work(2);
 exit(0);
}

//DElay  before creating process 3

for(volatile int i=0;i<1000000;i++);

//Create process 3

pid =fork();

if(pid==0){
 work(3);
 exit(0);
}



wait(0);
wait(0);
wait(0);

exit(0);
}


