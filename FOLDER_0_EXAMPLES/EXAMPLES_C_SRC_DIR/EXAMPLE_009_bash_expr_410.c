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
#define cr_whitespace(c) (whitespace(c) || ((c) == '\n'))

int myStatus=0;
int myStrlen = 0;

char *loopFunction(char *expr)
{
    char *p;

    // for (p = expr; p && *p && cr_whitespace(*p); p++)
    for (p = expr; *p && cr_whitespace(*p); p++)
    {
        ;
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
	if (myStatus != 0) { assert(0); }
	return 0;
}
