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

#define is_dir_sep(c) (((c) == '/') || ((c) == '\\'))

char *loopFunction(char *str)
{
    int namelen = strlen(str);
    while (namelen > 0 && !is_dir_sep(str[namelen - 1]))
    {
        namelen--;
    }
    return str + namelen;
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
