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

char *loopFunction(char *cp)
{
    // wget-1.9/src/http.c:2327:7
    char *ep;
    for (ep = cp; *ep && *ep != '\"'; ep++)
        ;
    return ep;
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
