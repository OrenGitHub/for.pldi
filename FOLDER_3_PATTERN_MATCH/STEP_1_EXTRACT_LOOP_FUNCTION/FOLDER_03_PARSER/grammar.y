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
	}
	gval;
}

/*************/
/* TERMINALS */
/*************/
%token <gval> FUNC

/*****************/
/* NON TERMINALS */
/*****************/
%type <gval> program

/**************/
/* START HERE */
/**************/
%start program

%%

program:	FUNC

%%

