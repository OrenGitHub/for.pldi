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

static inline  int crud(unsigned char c)
{
	return  c <= 32  ||
		c == '.' ||
		c == ',' ||
		c == ':' ||
		c == ';' ||
		c == '<' ||
		c == '>' ||
		c == '"' ||
		c == '\\' ||
		c == '\'';
}
char *loopFunction(char *src)
{
    int len = myStrlen;//strlen(src);
    char c;
    // git-2.18.0/ident.c:232:2
    while (len > 0)
    {
        c = src[len - 1];
        if (!crud(c))
        {
            break;
        }
        --len;
    }

    return src + len;
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
