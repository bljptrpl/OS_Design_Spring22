/*
 * Operating System Design
 * Daniel Villalva
 * code1
 * exec() example
 *
 */
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>


int main()
{
  pid_t firstPid, secondPid, thirdPid;

  firstPid = fork(); //parent

  if(firstPid < 0)
  {
    fprintf(stderr,"Error forking a process\n");
    return -1;
  }
  else if(firstPid == 0)
  {
	firstPid = fork(); //child
	if(firstPid == 0)
	{
		//child b/c 0
		execlp("echo", "echo", "Hello World", NULL);
		//wait();
	}
	else
	{
		//parent
		execlp("cat", "cat", "/proc/cpuinfo", NULL);
		//wait();
	}
  }//else if
  else
  {
	secondPid = fork(); //parent
    if(secondPid < 0)
    {
      fprintf(stderr,"Error forking a process\n");
      return -1;
    }
    else if(secondPid == 0)
    {
    	execlp("uname","uname","-a", NULL);
    	//wait();
    }
    else
    {
    	/* this is the parent, wait for the children to exit */
      	waitpid(firstPid,NULL,0);	/* don't read exit code */
    	waitpid(secondPid,NULL,0);	/* don't read exit code */
    }
  }
  thirdPid = fork(); //parent
  if(thirdPid < 0)
  {
    fprintf(stderr,"Error forking a process\n");
    return -1;
  }
  else if(thirdPid == 0)
  {
	  thirdPid = fork(); //child
	if(thirdPid == 0)
	{
		printf("Goodbye!\n");
		//exit(0);
	}
	else
	{
		execlp("ls","ls","-l", NULL);
		//wait();
	}
  }//else if


  return 0; /* this will invoke an exit() system call */
}
