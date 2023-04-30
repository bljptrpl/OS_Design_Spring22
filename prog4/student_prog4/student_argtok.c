//
//	EE 4374 Assignment #4 Argument Tokenizer
//	Author: Michael McGarry
//
#include <stdlib.h>

char ** argtok(char * argstr)
{
	int i, j;
	int counter;
	int tokBegin;		// index of beginning of token
	int prevDelim;		// flag to indicate whether previous character was a delimiter
	char **retValue;
	char *token;
	
	// Step 1: Count the tokens and then allocate the array of character pointers that will be used
	// to point to the individual tokens.
	i = -1;
	counter = 0;
	prevDelim = 0;
	do
	{
		// Point to next character
		i++;
		if((argstr[i] == 32) || (argstr[i] == 0))
		{
		    if(!prevDelim)
		    {
				counter++;			// count the token
			}
			prevDelim = 1;
		}
		else
		{
		  prevDelim = 0;
		}
	}
	while(argstr[i] != 0);
	retValue = malloc((counter+1)*sizeof(char *));
	
	// Step 2: For each token, count the characters and allocate memory, and copy the token
	i = -1;
	counter = 0;
	tokBegin = 0; // first token begins at index 0
	prevDelim = 0;
	do
	{
		// Point to next character
		i++;
		if((argstr[i] == 32) || (argstr[i] == 0))
		{
			if(!prevDelim)
			{
				// Allocate memory for the token and copy the token over
				token = malloc((i-tokBegin + 1)*sizeof(char)); // one extra character for the NULL
				for(j=0; j < (i-tokBegin); j++)
				{
					token[j] = argstr[tokBegin+j];
				}
				token[j] = 0;	// NULL terminate the string
				// Put the token on the token list
				retValue[counter] = token;
				counter++;	// point to next token
			}
			tokBegin = i+1; // next token begins after the whitespace
			prevDelim = 1;	// this character was a delimiter
		}
		else
		{
			prevDelim = 0;
		}
	}
	while(argstr[i] != 0);
	
	// NULL terminate the argument list
	retValue[counter] = 0;
	
	return retValue;

}