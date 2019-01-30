char *loopFunction(char *s)
{
 int l = strlen(s);
 while (l && s[l] != '/')
 {
  --l;
    }
 return s + l;
}