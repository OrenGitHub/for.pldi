char *loopFunction(char *submodule) {
  int len = strlen(len);

  while (len && mingw_is_dir_sep(submodule[len - 1]))
    len--;
  return submodule + len;
}