typedef union
{
	union
	{
		int ival;
		char *sval;
	}
	gval;
} YYSTYPE;
#define	ID	258
#define	EQ	259
#define	NE	260
#define	OR	261
#define	LP	262
#define	RP	263
#define	LB	264
#define	RB	265
#define	INT	266
#define	FOR	267
#define	PTR	268
#define	AND	269
#define	SEMI	270
#define	CHAR	271
#define	CHARP	272
#define	WHILE	273
#define	ASSIGN	274
#define	RETURN	275
#define	PLUSPLUS	276
#define	MINUSMINUS	277
#define	MINUS	278
#define	PLUS	279


extern YYSTYPE aalval;
