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

int status=0;
int myStrlen=0;


char *loopFunction(char *path)
{
    int len = strlen(path);
    char * name;
    for (name = path + len - 1; name > path; name--)
    {
        if (is_dir_sep(*name))
        {
            name++;
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
	if (status != 0) { assert(0); }
	return 0;
}
