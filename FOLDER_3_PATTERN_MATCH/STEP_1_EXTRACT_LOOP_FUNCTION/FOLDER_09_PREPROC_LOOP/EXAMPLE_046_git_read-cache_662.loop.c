char *loopFunction(char *path) {

  int namelen = strlen(path);
  while (namelen && path[namelen - 1] == '/')
    namelen--;

  return path + namelen;
}