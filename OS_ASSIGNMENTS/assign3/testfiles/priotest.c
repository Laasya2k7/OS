#include  "kernel/types.h"
#include"kernel/stat.h"
#include "user/user.h"

void work(int id){

volatile int x=0;

for(int i=0;i< 50000000; i++){
x+=i;
}
printf("Process %d completed\n",id);
}

int main(void){

int p1,p2,p3,p4;
p1=fork();

if(p1==0){
set_priority(getpid(), 10);
work(1);
exit(0);
}

p2=fork();

if(p2==0){
set_priority(getpid(), 5);
work(2);
exit(0);
}

p3=fork();

if(p3==0){
set_priority(getpid(),15);
work(3);
exit(0);
}

p4=fork();

if(p4==0){
set_priority(getpid(),20);
work(4);
exit(0);}


wait(0);
wait(0);
wait(0);
wait(0);
exit(0);
}

