char *loopFunction(char *s) {
  int len;

  for (len = 0; !sq_must_quote(s[len]); len++)
    ;
  return s + len;
}