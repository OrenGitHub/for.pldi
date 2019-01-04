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

/**********/
/* MACROS */
/**********/
#define whitespace(c) (((c) == ' ') || ((c) == '\t'))
#define cr_whitespace(c) (whitespace(c) || ((c) == '\n'))

int status=0;

char *loopFunction(char *xp)
{
  // while (xp && *xp && cr_whitespace(*xp))
  while (*xp && cr_whitespace(*xp))
  {
      xp++;
  }

  return xp;
}

int main(int argc, char **argv)
{
	char *s = malloc(N);
	klee_make_symbolic(s,N,"s");
	s[N-1]=0;
	printf("%s",loopFunction(s));
	if (status != 0) { assert(0); }
	return 0;
}
