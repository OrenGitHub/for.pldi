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

int myStatus=0;
int myStrlen=0;

char *loopFunction(char *name)
{
    // patch-2.7/lib/basename-lgpl.c:63:3
    int len;
    for (len = strlen(name); 1 < len && ISSLASH(name[len - 1]); len--)
       continue;
    return name + len;
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
