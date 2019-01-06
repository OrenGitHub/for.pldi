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

#define IN_CTYPE_DOMAIN(c) 1
#define ISDIGIT(c) (('0' <= c) && (c <= '9'))

int myStatus=0;

char *loopFunction(char *f)
{
    char c = *f++;
    while (ISDIGIT(c))
        c = *f++;
    return f;
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
