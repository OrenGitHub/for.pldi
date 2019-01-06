/*****************/
/* INCLUDE FILES */
/*****************/
#include <stdio.h>
#include <stdbool.h>
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



bool
c_isspace (int c)
{
  switch (c)
    {
    case ' ': case '\t': case '\n': case '\v': case '\f': case '\r':
      return true;
    default:
      return false;
    }
}

char *loopFunction(char *p) {
  // tar-1.29/gnu/parse-datetime.y:1283:3
  char c;
  while (c = *p, c_isspace(c))
    p++;
  return p;
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

#ifdef MAIN_FUNC
#include "main.c"
#endif
