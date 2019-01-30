# 1 "/home/oren/GIT/for.pldi/FOLDER_0_EXAMPLES/EXAMPLES_C_SRC_DIR/EXAMPLE_115_bash_file_73.c"
# 1 "<built-in>"
# 1 "<command-line>"
# 31 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 32 "<command-line>" 2
# 1 "/home/oren/GIT/for.pldi/FOLDER_0_EXAMPLES/EXAMPLES_C_SRC_DIR/EXAMPLE_115_bash_file_73.c"


int unquoted_tilde_word (const char *s)
{
 const char *r;

 for (r = s; ((*r) == '\0' || (*r) == '/' || (*r) == ':') == 0; r++)
    {
  switch (*r)
  {
   case '\\':
   case '\'':
   case '"':
    return 0;
  }
 }
 return 1;
}

int main(int argc, char **argv)
{
 return unquoted_tilde_word(argv[1]);
}
