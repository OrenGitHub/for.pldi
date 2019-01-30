%{
/*************************/
/* GENERAL INCLUDE FILES */
/*************************/
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

/*************************/
/* PROJECT INCLUDE FILES */
/*************************/
#include "util.h"
#include "ErrorMsg.h"

/**********/
/* ALLOCA */
/**********/
#define alloca malloc

/*******************/
/* LEXING FUNCTION */
/*******************/
int aalex(void);

/******************/
/* ERROR FUNCTION */
/******************/
void aaerror(char *s)
{
	ErrorMsg_Error(ErrorMsg_tokPos,"%s",s);
}

%}

/********************************************************************************/
/* This union is a C replacement for an abstract class representing an AST node */
/********************************************************************************/
%union
{
	union
	{
		int ival;
	}
	gval;
}

/*************/
/* TERMINALS */
/*************/
%token <gval> LINE

/*****************/
/* NON TERMINALS */
/*****************/
%type <gval> program
%type <gval> lines

/**************/
/* START HERE */
/**************/
%start program

%%

char* loopFunction(char* line)
{
    char *p;

    for (p = line; p && *p && (((*p) == ' ') || ((*p) == '\t')); p++);


    return p;
}


program:	func

func:		CHARP FNAME LP CHARP ID RP LB stmts RB

stmts:		stmt stmts
			| stmt

stmt:		stmtEmpty
			| stmtDec
			| stmtLoop
			| stmtReturn

stmtDec:	ID ID SEMI
stmtLoop:	FOR LP init cond finialize RP stmts
stmtEmpty:	SEMI
stmtReturn:	RETURN ID SEMI


%%

