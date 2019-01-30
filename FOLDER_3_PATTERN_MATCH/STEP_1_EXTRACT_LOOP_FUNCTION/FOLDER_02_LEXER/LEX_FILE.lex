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

/***************/
/* SINGLE LINE */
/***************/
LINE			[^\r\n]*
LINE_TERMINATOR	\n

/***************/
/* PARENTHESES */
/***************/
RB		"}"
RBS		("\n}")

/*************/
/* LOOP_FUNC */
/*************/
LOOP_FUNC	[^\r\n]+"loopFunction(c"

/*********/
/* NO_RB */
/*********/
NO_RB		[^}]+
		
/*********/
/* RULES */
/*********/
%%
{LINE}{LINE_TERMINATOR}				{adjust(); ErrorMsg_Newline();   continue;   }
{LOOP_FUNC}{NO_RB}{RBS}				{adjust(); ErrorMsg_Log(aatext); return FUNC;}
{LOOP_FUNC}{NO_RB}{RB}{NO_RB}{RBS}	{adjust(); ErrorMsg_Log(aatext); return FUNC;}
.										{
											adjust();
											ErrorMsg_Log(">> ERROR");
										}

