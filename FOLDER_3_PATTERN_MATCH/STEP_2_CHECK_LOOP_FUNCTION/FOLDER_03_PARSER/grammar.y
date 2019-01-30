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
	printf("0\n");
	// ErrorMsg_Error(ErrorMsg_tokPos,"%s",s);
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
		char *sval;
	}
	gval;
}

/*************/
/* TERMINALS */
/*************/
%token <gval> ID
%token <gval> EQ
%token <gval> NE
%token <gval> OR
%token <gval> LP
%token <gval> RP
%token <gval> LB
%token <gval> RB
%token <gval> INT
%token <gval> FOR
%token <gval> PTR
%token <gval> AND
%token <gval> SEMI
%token <gval> CHAR
%token <gval> CHARP
%token <gval> WHILE
%token <gval> ASSIGN
%token <gval> RETURN
%token <gval> PLUSPLUS
%token <gval> MINUSMINUS

/*****************/
/* NON TERMINALS */
/*****************/
%type <gval> program
%type <gval> func
%type <gval> stmts
%type <gval> stmt
%type <gval> stmtEmpty
%type <gval> stmtWhile
%type <gval> stmtFor
%type <gval> stmtInc
%type <gval> stmtDec
%type <gval> stmtReturn
%type <gval> varDec
%type <gval> pre
%type <gval> cond
%type <gval> post

/******************************/
/* ASSOCIATIVITY & PRECEDENCE */
/******************************/
%left OR,AND,EQ
%left MINUS,PLUS

/**************/
/* START HERE */
/**************/
%start program

%%

program:	func

func:		CHARP ID LP CHARP ID RP LB stmts RB

stmts:		stmt stmts
			| stmt

stmt:		stmtEmpty
			| stmtWhile
			| stmtFor
			| stmtInc
			| stmtDec
			| stmtReturn
			| varDec

varDec:		ID ID SEMI
			| ID PTR ID SEMI
			| CHARP ID SEMI

stmtEmpty:	SEMI
stmtWhile:	WHILE LP cond RP LB stmts RB
			| WHILE LP cond RP stmt
stmtFor:	FOR LP pre SEMI cond SEMI post RP LB stmts RB
			| FOR LP     SEMI cond SEMI post RP LB stmts RB
			| FOR LP pre SEMI cond SEMI post RP stmt
			| FOR LP     SEMI cond SEMI post RP stmt
stmtReturn:	RETURN ID SEMI
stmtInc:	ID PLUSPLUS SEMI
stmtDec:	ID MINUSMINUS SEMI

cond:		PTR ID
			| LP cond RP
			| cond EQ CHAR
			| cond NE CHAR
			| cond AND cond
			| cond OR  cond

post:		ID PLUSPLUS

pre:		ID ASSIGN ID

%%

