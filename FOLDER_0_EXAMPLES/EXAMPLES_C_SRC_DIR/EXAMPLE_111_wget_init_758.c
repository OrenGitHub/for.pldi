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

int myStatus = 0;
int myStrlen = 0;

# define ISSEP(c) ((c) == '/')

char *loopFunction(char *val)
{
    // wget-1.9/src/init.c:758:7
    for (++val; ISSEP(*val); val++);
    return val;
}

int main(int argc, char **argv)
{
	char *s = malloc(N);
	klee_make_symbolic(s,N,"s");
	s[N-1]=0;
	printf("%s",loopFunction(s));
	if (myStatus != 0) { assert(0); }
	return 0;
}
