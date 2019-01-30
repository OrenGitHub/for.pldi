# 1 "/home/oren/GIT/for.pldi/FOLDER_0_EXAMPLES/EXAMPLES_C_SRC_DIR/EXAMPLE_116_bash_file_744.c"
# 1 "<built-in>"
# 1 "<command-line>"
# 31 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 32 "<command-line>" 2
# 1 "/home/oren/GIT/for.pldi/FOLDER_0_EXAMPLES/EXAMPLES_C_SRC_DIR/EXAMPLE_116_bash_file_744.c"
static int should_expand (char *s)
{
 char *p;

 for (p = s; p && *p; p++)
 {
  if (*p == '\\')
  {
   p++;
  }
  else if (*p == '&')
  {
   return 1;
  }
 }
 return 0;
}

int main(int argc, char **argv)
{
 return should_expand(argv[1]);
}
