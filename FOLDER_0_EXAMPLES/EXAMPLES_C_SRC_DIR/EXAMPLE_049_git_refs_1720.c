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

static inline int mingw_is_dir_sep(int c)
{
	return c == '/' || c == '\\';
}
#define is_dir_sep mingw_is_dir_sep
char *loopFunction(char *submodule) {
  int len = strlen(len);
  // git-2.18.0/refs.c:1720:2
  while (len && is_dir_sep(submodule[len - 1]))
    len--;
  return submodule + len;
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
