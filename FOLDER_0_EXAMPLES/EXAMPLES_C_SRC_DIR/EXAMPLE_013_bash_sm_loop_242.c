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
#define N 4

int myStatus = 0;
int myStrlen = 0;

#define L(CS) CS

char* loopFunction(char* n)
{
    // doesn't compile
    // char* se = strlen(n);
    // added this:
    char *se = n+strlen(n);
    while (n < se && *n != L('/'))
        ++n;
    // added this:
    return n;
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
