#include <stdio.h>
#include <string.h>
#define N 10

char *loopFunction(char *url) {
  // wget-1.9/src/url.c:532:3
  char *p;
  for (p = url; *p && *p != ':' && *p != '/'; p++)
    ;
  return p;
}

void driver() {
  char str[N];
  klee_make_symbolic(str, sizeof(str), "str");

  char *p = loopFunction(str);
#ifdef DRIVER
#include "driver.c"
#endif
}

#ifdef MAIN_FUNC
#include "main.c"
#endif
