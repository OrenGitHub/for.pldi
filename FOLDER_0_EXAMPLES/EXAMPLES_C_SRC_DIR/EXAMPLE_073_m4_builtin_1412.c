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

int status=0;
int myStrlen=0;

char *loopFunction(char *pattern)
{

    int len = strlen(pattern);
    int i;
    for (i = 0; len > 0 && i < 6; i++)
        if (pattern[len - i - 1] != 'X')
          break;
    return pattern + i;
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
