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

#define ISDIGIT(c) ((unsigned) (c) - '0' <= 9)

int status=0;
int myStrlen=0;

char *loopFunction(char *p)
{
    // patch-2.7/src/pch.c:2306:7
    while (ISDIGIT(*++p))
    {
        /* do nothing */;
    }
    return p;
}

int main(int argc, char **argv)
{
	char *s = malloc(N);
	klee_make_symbolic(s,N,"s");
	s[N-1]=0;
	myStrlen = strlen(s);
	printf("%s",loopFunction(s));
	if (status != 0) { assert(0); }
	return 0;
}
