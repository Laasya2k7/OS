#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void work(int id, int burst){
volatile int x=0;

for(int i=0;i<burst*10000000;i++){
x+=i;
}

printf("Process %d (burst %d) completed\n", id, burst
);
}

int main(void){
int p1, p2, p3, p4;

set_burst(getpid(),1);

p1=fork();

if(p1==0){
work(1,40);
exit(0);}
set_burst(getpid(),40);

p2=fork();

if(p2==0){
work(2,10);
exit(0);
}
set_burst(getpid(),10);




p3=fork();

if(p3==0){
work(3,25);
exit(0);
}
set_burst(getpid(),25);
p4=fork();

if(p4==0){
work(4,5);
exit(0);
}
set_burst(getpid(),5);

wait(0);
wait(0);
wait(0);
wait(0);

exit(0);
}
