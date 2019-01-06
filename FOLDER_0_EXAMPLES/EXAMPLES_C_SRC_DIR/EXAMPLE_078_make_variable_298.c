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

int myStatus = 0;
int myStrlen = 0;

#define ISDIGIT(c) ((unsigned) (c) - '0' <= 9)

char *loopFunction(char *name)
{
    for (++name; *name != '\0'; ++name)
    {
        if (*name != '_' && (*name < 'a' || *name > 'z') && 
           (*name < 'A' || *name > 'Z') && !ISDIGIT(*name))
        {
            break;
        }
    }
    return name;
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
