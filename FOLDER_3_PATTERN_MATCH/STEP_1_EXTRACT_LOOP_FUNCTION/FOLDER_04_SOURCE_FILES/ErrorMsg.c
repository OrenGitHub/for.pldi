/*************************/
/* FILE NAME: ErrorMsg.c */
/*************************/

/****************************************/
/* WARNING DISABLE: sprintf - I love it */
/****************************************/
#pragma warning (disable: 4996)

/*************************/
/* GENERAL INCLUDE FILES */
/*************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

/*************************/
/* PROJECT INCLUDE FILES */
/*************************/
#include "util.h"
#include "ErrorMsg.h"

/***************/
/* DEFINITIONS */
/***************/
#define MAX_FILENAME_LENGTH 512

/****************/
/* LOG FILENAME */
/****************/
char ErrorMsg_Log_Filename[MAX_FILENAME_LENGTH];

/********************/
/* GLOBAL VARIABLES */
/********************/
static bool anyErrors= FALSE;
static string fileName = "";
static int lineNum = 1;

/********************/
/* GLOBAL VARIABLES */
/********************/
int ErrorMsg_tokPos=0;

/**********************/
/* EXTERNAL VARIABLES */
/**********************/
extern FILE *aain;

/*********/
/* TYPES */
/*********/
typedef struct intList {int i; struct intList *rest;} *IntList;

/*********/
/* CTORS */
/*********/
static IntList intList(int i, IntList rest) 
{
	IntList l= (IntList) checked_malloc(sizeof *l);
	l->i=i; l->rest=rest;
	
	return l;
}

/********************/
/* GLOBAL VARIABLES */
/********************/
static IntList linePos=NULL;

/********************/
/* GLOBAL VARIABLES */
/********************/
FILE *ErrorMsg_Log_fl;

/****************/
/* LOG FUNCTION */
/****************/
void ErrorMsg_Log(char *message,...)
{
	/***********/
	/* va_list */
	/***********/
	va_list ap;

	/*********************************************/
	/* Reset the file content from previous runs */
	/*********************************************/
	static int first_time=1;

	if (first_time == 1) {
		first_time  = 0;
		ErrorMsg_Log_fl = fopen(ErrorMsg_Log_Filename,"w+t");
		if (ErrorMsg_Log_fl == NULL) return;
		fclose(ErrorMsg_Log_fl);
	}

	/*************************/
	/* For each LEXED token: */
	/* [1] open the log file */
	/* [2] write the token   */
	/* [3] close the file    */
	/*************************/
	/*************************/
	/* [1] Open the log file */
	/*************************/
	ErrorMsg_Log_fl = fopen(ErrorMsg_Log_Filename,"a+t");
	if (ErrorMsg_Log_fl == NULL) return;
		
	/*************************/
	/* [2] Write LEXED token */
	/*************************/
	va_start(ap,message);
	vfprintf(ErrorMsg_Log_fl, message, ap);
	va_end(ap);

	/**********************/
	/* [3] Close the file */
	/**********************/
	fclose(ErrorMsg_Log_fl);
}

/***********/
/* NEWLINE */
/***********/
void ErrorMsg_Newline(void)
{
	lineNum++;
	linePos = intList(ErrorMsg_tokPos, linePos);
}

/*********/
/* ERROR */
/*********/
void ErrorMsg_Error(int pos, char *message,...)
{
	va_list ap;
	IntList lines = linePos;
	int num=lineNum;

	anyErrors=TRUE;
	while (lines && lines->i >= pos) 
	{
		lines=lines->rest;
		num--;
	}

	if (fileName)
	{
		fprintf(stderr,"%s:",fileName);
	}
	
	if (lines)
	{
		fprintf(stderr,"%d.%d: ", num, pos-lines->i);
	}
	
	va_start(ap,message);
	vfprintf(stderr, message, ap);
	va_end(ap);
	fprintf(stderr,"\n");

	/*********************************/
	/* [0] exit at the first mistake */
	/*********************************/
	exit(0);
}

/****************/
/* LOG FILENAME */
/****************/
void ErrorMsg_Set_Log_Filename(string log_Filename)
{
	strcpy(ErrorMsg_Log_Filename,log_Filename);
}

/*********/
/* RESET */
/*********/
void ErrorMsg_Reset(string fname)
{
	anyErrors=FALSE;
	fileName=fname;
	lineNum=1;
	linePos=intList(0,NULL);
	aain = fopen(fname,"r");
	if (!aain)
	{
		ErrorMsg_Error(0,"cannot open");
		exit(1);
	}
}

