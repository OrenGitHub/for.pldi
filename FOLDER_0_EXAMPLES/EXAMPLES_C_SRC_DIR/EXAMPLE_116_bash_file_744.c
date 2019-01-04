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
