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

int myStatus=0;
int myStrlen=0;

char *loopFunction(char *remote)
{
    char* ptr;
    int len;
    // for (len = 0, ptr = remote + strlen(remote); remote < ptr && ptr[-1] == '^'; ptr--)
    for (len = 0, ptr = remote + myStrlen; remote < ptr && ptr[-1] == '^'; ptr--)
    {
        len++;
    }
    return ptr;
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
