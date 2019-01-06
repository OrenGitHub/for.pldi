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
#define N 5


int myStatus=0;
int myStrlen=0;

char *loopFunction(char *str)
{
    // patch-2.7/src/pch.c:387:3
    char *s;
    for (s = str; (*s >= '0' && *s <= '9') || (*s >= 'a' && *s <= 'f'); s++)
        /* do nothing */;
    return s;
}

int main(int argc, char **argv)
{
	char *s = malloc(N);
	klee_make_symbolic(s,N,"s");
	s[N-1]=0;
	myStrlen = strlen(s);
	printf("%s",loopFunction(s));
	if (myStatus != 0) { assert(0); }
	return 0;
}
