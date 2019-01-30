%{
/*************************/
/* GENERAL INCLUDE FILES */
/*************************/
#include <math.h>
#include <string.h>
#include <stdlib.h>

/*************************/
/* PROJECT INCLUDE FILES */
/*************************/
#include "util.h"
#include "ErrorMsg.h"
#include "Parser.h"

/**************************/
/* CONTROL ERROR MESSAGES */
/**************************/
static int charPos=1;

/******************/
/* PROVIDE aaWRAP */
/******************/
int aawrap(void)
{
	charPos=1;
	return 1;
}

/**************************/
/* CONTROL ERROR MESSAGES */
/**************************/
static void adjust(void)
{
	ErrorMsg_tokPos = charPos;
	charPos += aaleng;
}

%}

/*****************/
/* UNIQUE PREFIX */
/*****************/
%option prefix="aa"

/********************/
/* COMMON REGEXP(s) */
/********************/

/**********/
/* SPACES */
/**********/
TAB				\t
WHITE			" "
LINE_TERMINATOR	\n

/***************/
/* PARENTHESES */
/***************/
LP	"("
RP	")"
LB	"{"
RB	"}"

/**********/
/* BINOPS */
/**********/
PLUS	"+"
MINUS	"-"
PTR		"*"
AND		"&&"
OR		"||"
EQ		"=="
NE		"!="
ASSIGN	"="

/*********/
/* UNOPS */
/*********/
PLUSPLUS	"++"
MINUSMINUS	"--"

/*********/
/* OTHER */
/*********/
CHARP	"char *"|"char*"
WHILE	"while"
RETURN	"return"
FOR		"for"
SEMI	";"
CHAR	"' '"|"'\\t'"|"'\\n'"|"'\\\\'"|"'/'"|"'='"

/*******/
/* INT */
/*******/
INT		"0"|[1-9][0-9]*

/******/
/* ID */
/******/
ID		[a-zA-Z_]+
		
/*********/
/* RULES */
/*********/
%%
{TAB}				{adjust(); ErrorMsg_Log("\t"); continue;}
{WHITE}				{adjust(); ErrorMsg_Log(" " ); continue;}
{LINE_TERMINATOR}	{adjust(); ErrorMsg_Newline(); continue;}
{LP}				{adjust(); ErrorMsg_Log("("      ); return LP;}
{RP}				{adjust(); ErrorMsg_Log(")"      ); return RP;}
{LB}				{adjust(); ErrorMsg_Log("{"      ); return LB;}
{RB}				{adjust(); ErrorMsg_Log("}"      ); return RB;}
{EQ}				{adjust(); ErrorMsg_Log("=="     ); return EQ;}
{NE}				{adjust(); ErrorMsg_Log("!="     ); return NE;}
{OR}				{adjust(); ErrorMsg_Log("||"     ); return OR;}
{AND}				{adjust(); ErrorMsg_Log("&&"     ); return AND;}
{PTR}				{adjust(); ErrorMsg_Log("*"      ); return PTR;}
{FOR}				{adjust(); ErrorMsg_Log("for"    ); return FOR;}
{SEMI}				{adjust(); ErrorMsg_Log(";"      ); return SEMI;}
{PLUS}				{adjust(); ErrorMsg_Log("+"      ); return PLUS;}
{MINUS}				{adjust(); ErrorMsg_Log("-"      ); return MINUS;}
{WHILE}				{adjust(); ErrorMsg_Log("while"  ); return WHILE;}
{ASSIGN}			{adjust(); ErrorMsg_Log("="      ); return ASSIGN;}
{RETURN}			{adjust(); ErrorMsg_Log("return" ); return RETURN;}
{CHARP}				{adjust(); ErrorMsg_Log("char *" ); return CHARP;}
{PLUSPLUS}			{adjust(); ErrorMsg_Log("++"     ); return PLUSPLUS;}
{MINUSMINUS}		{adjust(); ErrorMsg_Log("--"     ); return MINUSMINUS;}
{INT}				{
						adjust();
						aalval.gval.ival=atoi(aatext);
						ErrorMsg_Log("INT(%d) ",aalval.gval.ival);
						return INT;
					}
{ID}				{
						adjust();
						aalval.gval.sval=String(aatext);
						ErrorMsg_Log("%s",aalval.gval.sval);
						return ID;
					}
{CHAR}				{
						adjust();
						aalval.gval.sval=String(aatext);
						ErrorMsg_Log("%s",aalval.gval.sval);
						return CHAR;
					}
.					{
						adjust();
						ErrorMsg_Log(">> ERROR");
					}

