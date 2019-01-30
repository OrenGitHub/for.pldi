char *loopFunction(char *s) {
  int len;
  int maxlen = strlen(s);

  for (len = 0; len < maxlen && !sq_must_quote(s[len]); len++)
    ;
  return s + len;
}