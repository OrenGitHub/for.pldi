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

int is_urlschemechar(int first_flag, int ch)
{
	/*
	 * The set of valid URL schemes, as per STD66 (RFC3986) is
	 * '[A-Za-z][A-Za-z0-9+.-]*'. But use sightly looser check
	 * of '[A-Za-z0-9][A-Za-z0-9+.-]*' because earlier version
	 * of check used '[A-Za-z0-9]+' so not to break any remote
	 * helpers.
	 */
	int alphanumeric, special;
	alphanumeric = ch > 0 && isalnum(ch);
	special = ch == '+' || ch == '-' || ch == '.';
	return alphanumeric || (!first_flag && special);
}
char *loopFunction(char *url) {
  char* p = url;
  // git-2.18.0/transport.c:892:3
  while (is_urlschemechar(p == url, *p))
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
