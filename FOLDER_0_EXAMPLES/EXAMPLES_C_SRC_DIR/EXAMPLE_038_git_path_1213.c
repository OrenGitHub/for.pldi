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

int status = 0;
int myStrlen = 0;

#define is_dir_sep(c) (((c) == '/') || ((c) == '\\'))

char *loopFunction(char *path)
{
    int len = strlen(path);
    // git-2.18.0/path.c:1213:2
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
	s[0]='a';
	s[1]='b';
	s[2]='c';
	s[3]='d';
	s[4]='e';
	s[5]= 0 ;
	myStrlen = strlen(s);
	printf("%s",loopFunction(s));
	if (status != 0) { assert(0); }
	return 0;
}
