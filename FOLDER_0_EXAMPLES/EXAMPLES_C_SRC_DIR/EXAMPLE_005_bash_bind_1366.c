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

int status=0;

char *loopFunction(char *value)
{
    char *e = value + strlen(value) - 1;
    
    while (e >= value && whitespace(*e))
    {
        e--;
    }
    return e;
}

int main(int argc, char **argv)
{
	char *s = malloc(N);
	klee_make_symbolic(s,N,"s");
	printf("%s",loopFunction(s));
	if (status != 0) { assert(0); }
	return 0;
}
