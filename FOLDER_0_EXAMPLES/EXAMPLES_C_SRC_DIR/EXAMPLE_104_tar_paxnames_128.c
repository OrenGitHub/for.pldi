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

#define ISSLASH(C) ((C) == '/' || (C) == '\\')
char *loopFunction(char *p) {
  // tar-1.29/lib/paxnames.c:128:7
  for (; ISSLASH(*p); p++)
    continue;
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
