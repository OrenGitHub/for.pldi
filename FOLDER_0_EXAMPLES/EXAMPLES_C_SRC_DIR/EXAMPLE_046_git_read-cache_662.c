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

int myStatus = 0;
int myStrlen = 0;

char *loopFunction(char *path) {
  // git-2.18.0/read-cache.c:662:3
  int namelen = strlen(path);
  while (namelen && path[namelen - 1] == '/')
    namelen--;

  return path + namelen;
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
