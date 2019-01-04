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

int status = 0;
int myStrlen = 0;

# define ISSLASH(C) ((C) == '/' || (C) == '\\')
char *loopFunction(char *pattern)
{
    // tar-1.29/gnu/exclude.c:552:8
    int len = strlen(pattern);
    while (len > 0 && ISSLASH(pattern[len - 1]))
    {
        --len;
    }
    return pattern + len;
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
