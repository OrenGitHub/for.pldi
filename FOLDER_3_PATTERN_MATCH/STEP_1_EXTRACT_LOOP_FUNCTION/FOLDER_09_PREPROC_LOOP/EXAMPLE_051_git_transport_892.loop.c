char *loopFunction(char *url) {
  char* p = url;

  while (is_urlschemechar(p == url, *p))
    p++;
  return p;
}