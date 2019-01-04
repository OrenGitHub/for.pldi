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

# define ISSLASH(C) ((C) == '/' || (C) == '\\')

int status=0;
int myStrlen=0;

char *loopFunction(char *t)
{
    // patch-2.7/src/util.c:347:4
    char *o;
    int tlen = strlen(t);
    int olen;
    for (o = t + tlen, olen = 0; o > t && !ISSLASH(*(o - 1)); o--)
    {
        /* do nothing */;
    }
    return o;
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
