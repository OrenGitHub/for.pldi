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

int myStatus=0;
int myStrlen=0;

#define is_dir_sep(c) (((c) == '/') || ((c) == '\\'))

char *loopFunction(char *end)
{
    for (; *end && !is_dir_sep(*end); end++)
        ; /* nothing */
    return end;
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
