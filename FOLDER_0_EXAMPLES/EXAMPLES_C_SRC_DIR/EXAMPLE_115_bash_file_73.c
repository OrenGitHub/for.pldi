#define TILDE_END(c) ((c) == '\0' || (c) == '/' || (c) == ':')

int unquoted_tilde_word (const char *s)
{
	const char *r;

	for (r = s; TILDE_END(*r) == 0; r++)
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
