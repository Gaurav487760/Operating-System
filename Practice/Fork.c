#include <stdio.h>
#include <unistd.h>
#include<stdlib.h>
int main()
{
int pid;
pid=fork();
if(pid<0)
{
printf("\n Error in creation of Child Process ");
exit(1);
}
else if(pid==0)
{
printf("\n Hello, I am the child process");
printf("\n My pid is %d ",getpid());
exit(0);
}
else
{
printf("\n Hello, I am the parent process ");
printf("\n My pid is %d \n ",getpid());
exit(1);
}
}
