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

/**********/
/* MACROS */
/**********/
#define whitespace(c) (((c) == ' ') || ((c) == '\t'))

/********************/
/* GLOBAL VARIABLES */
/********************/
int status=0;
int myStrlen = 0;

char *loopFunction(char *s)
{
    // while (s && *s && whitespace(*s))
    while (*s && whitespace(*s))
        s++;
    return s;
}

int main(int argc, char **argv)
{
	char *s = malloc(N);
	klee_make_symbolic(s,N,"s");
	s[N-1]=0;
	printf("%s",loopFunction(s));
	if (status > 10) { assert(0); }
	return 0;
}
