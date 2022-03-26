// UT El Paso EE 4374 Programming Assignment 2
// Multi-threaded Prime Number Search
// Author: Daniel Villalva
//
#include <pthread.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include "dvillalva_prime.h"

#define BUFFER_SIZE	1000000

unsigned char buffer[BUFFER_SIZE+1];
unsigned char fileName[100];
FILE *primeFile;
FILE *primeThreadFile;

// could make int j a global and have both counters run at the same time, saving time.

int test_prime(int n)
{
	for(int i = 2; i < n; i++)
	{
		if(n % i == 0)//NP
		{
			return 0;
		}
		else //Prime
		{
			//printf("number is %d, and prime\n", n);
			return 1;
		}
	}//for
}//func

void *prime_search(void *param)
{
	for( int j = 2; j < 5000000; j++)
	{
		param = (void *)j;
		primeThreadData[0].current = param;
		sleep(1/10000000000000); //goes to fast if no delay to show count in minishell
		//printf("numbers[%d] is recording in numbers...%d \n", param, j);
	}
}
int main(int argc, char *argv[])
{
  int i, bytesRead, bytesWritten;
  pthread_t tid[MAX_THREADS]; 
  pthread_t tidshell;
  pthread_attr_t attr;
  time_t before, after;

  int *numbers1;

  /* Record time at start */
  before = time(NULL);

  numThreads = 2;
  
  /* Launch mini shell thread to provide interactivity with the user */
  pthread_create(&tidshell,NULL,mini_shell,NULL);
  
  /* Launch threads to find prime numbers */
  pthread_create(&tid[0], NULL, prime_search,&numbers1);
  pthread_create(&tid[1], NULL, test_prime,NULL);
  for( int k = 2; k < 5000000; k++)
  {
	  int test;
	  primeThreadData[1].current = k;
	  test = test_prime(k);
	  if(test == 1)
	   {
		  //printf("number %d prime %d\n", k, test);//print prime numbers to page
		  primeThreadFile = fopen("primet.txt","a");
		  fprintf(primeThreadFile, "%d\n", k);
		  fclose(primeThreadFile);

	   }//if
  }//for

	  pthread_join(tid[0], NULL);
	  pthread_join(tid[1], NULL);
	  after = time(NULL);
	  printf("\nPrime search done after %ld seconds\n", after-before);

#if 0 /* This code segment will merge the output of the two prime search threads, use it if you like */

  /* Create primes output file */
  primeFile = fopen("primest","w");
  fclose(primeFile);

  /* Wait for the prime search threads to complete and combine their data */
  for(i = 0; i < numThreads; i++)
  {
  	/* Wait for the next thread to complete */
  	pthread_join(tid[i],NULL);
  	/* On thread completion, write its data to "primest" */
    fileName[0] = '\0';
    sprintf(fileName, "primes%d", i+1);					// Open the thread's data file
    if((primeThreadFile = fopen(fileName,"r")) == NULL)
    {
    	printf("Failed to open file: %s\n", fileName);
    }
    else
    {
    	if((primeFile = fopen("primest","a")) == NULL)	// Open "primest"
    	{
    		printf("Failed to open file: primest\n");
    	}
    	else
    	{
    		while(feof(primeThreadFile) == 0)
			{
				/* Read from the thread's data file */
				bytesRead = fread(buffer,1,BUFFER_SIZE,primeThreadFile);
				/* Write to the combined file */
				bytesWritten = fwrite(buffer,1,bytesRead,primeFile);
    		}
			fclose(primeFile);
    	}
		fclose(primeThreadFile);
	}
  }
  
  /* Record execution time */
  after = time(NULL);
  printf("\nPrime search done after %ld seconds\n", after-before);
#endif

  sleep(20);
  
  /* Lastly, kill the interaction thread */
  pthread_kill(tidshell, SIGKILL);

  return 0;

}

