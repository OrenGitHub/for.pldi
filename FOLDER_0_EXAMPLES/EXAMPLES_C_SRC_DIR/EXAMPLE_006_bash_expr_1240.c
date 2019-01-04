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

/**********/
/* MACROS */
/**********/
#define whitespace(c) (((c) == ' ') || ((c) == '\t'))

int status=0;

#define cr_whitespace(c) (whitespace(c) || ((c) == '\n'))

char *loopFunction(char *cp)
{
    char c;
    // while (cp && (c = *cp) && (cr_whitespace(c)))
    while ((c = *cp) && (cr_whitespace(c)))
    {
        cp++;
    }   
    return cp;
}

int main(int argc, char **argv)
{
	char *s = malloc(N);
	klee_make_symbolic(s,N,"s");
	s[N-1]=0;
	printf("%s",loopFunction(s));
	if (status != 0) { assert(0); }
	return 0;
}
