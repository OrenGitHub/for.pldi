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

#define is_dir_sep(c) (((c) == '/') || ((c) == '\\'))

int myStatus=0;
int myStrlen=0;

char *loopFunction(char *path)
{
    int len = strlen(path);
    while (len && is_dir_sep(path[len - 1]))
    {
        len--;
    }
    return path + len;
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
