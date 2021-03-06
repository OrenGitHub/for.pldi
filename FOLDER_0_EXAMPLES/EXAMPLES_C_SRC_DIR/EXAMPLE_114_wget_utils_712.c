/*****************/
/* INCLUDE FILES */
/*****************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

/*****************/
/* INCLUDE FILES */
/*****************/
#include "klee/klee.h"

/***************/
/* DEFINITIONS */
/***************/
#define N 10

/********************/
/* GLOBAL VARIABLES */
/********************/
int myStatus=0;
int myStrlen = 0;

char *loopFunction(char *s)
{
	int l = strlen(s);
	while (l && s[l] != '/')
	{
		--l;
    }
	return s + l;
}

int main(int argc, char **argv)
{
	char *s = malloc(N);
	klee_make_symbolic(s,N,"s");
	s[N-1]=0;
	printf("%s",loopFunction(s));
	if (myStatus > 10) { assert(0); }
	return 0;
}
