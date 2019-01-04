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

# define _IS_DRIVE_LETTER(C) (((unsigned int) (C) | ('a' - 'A')) - 'a'  \
                              <= 'z' - 'a')
# define FILE_SYSTEM_PREFIX_LEN(Filename) \
          (_IS_DRIVE_LETTER ((Filename)[0]) && (Filename)[1] == ':' ? 2 : 0)
# define ISSLASH(C) ((C) == '/' || (C) == '\\')

int status=0;
int myStrlen=0;

char *loopFunction(char *filename)
{
    // patch-2.7/src/util.c:1230:3
    char *f;
    for (f = filename + FILE_SYSTEM_PREFIX_LEN(filename); ISSLASH(*f); f++)
    {
        /* do nothing */;
    }
    return f;
}

int main(int argc, char **argv)
{
	char *s = malloc(N);
	klee_make_symbolic(s,N,"s");
	s[N-1]=0;
	s[0]='c';
	s[1]=':';
	s[2]='P';
	s[3]= 0 ;
	myStrlen = strlen(s);
	printf("%s",loopFunction(s));
	if (status != 0) { assert(0); }
	return 0;
}
