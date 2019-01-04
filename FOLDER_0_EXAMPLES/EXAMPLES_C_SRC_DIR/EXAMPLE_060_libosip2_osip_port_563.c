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


int status=0;
int myStrlen=0;

char *loopFunction(char *pbeg)
{
    // libosip2-4.1.0/src/osipparser2/osip_port.c:563:3
    while ((' ' == *pbeg) || ('\r' == *pbeg) || ('\n' == *pbeg) || ('\t' == *pbeg))
    {
        pbeg++;
    }
    return pbeg;
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
