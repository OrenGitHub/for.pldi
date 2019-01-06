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

char *loopFunction(char *in)
{
    int j = 0;
    // git-2.18.0/path.c:1044:2
    while (is_dir_sep(in[j]))
    {
        j++;
    }

    return in + j;
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
