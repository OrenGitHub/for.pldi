/****************/
/* Parser_API.c */
/****************/

/*************************/
/* GENERAL INCLUDE FILES */
/*************************/
#include <stdio.h>

/*************************/
/* PROJECT INCLUDE FILES */
/*************************/
#include "util.h"
#include "ErrorMsg.h"
#include "Parser_API.h"

/**********************/
/* Parse Input Matrix */
/**********************/
int Parse(string Input_Filename)
{
	/*************************/
	/* Open file for parsing */
	/*************************/
	ErrorMsg_Reset(Input_Filename);

	/*******************************/
	/* 3 ... 2 ... 1 ... parse !!! */
	/*******************************/
	if (aaparse() == 0)
	{
		/******************/
		/* parsing worked */
		/******************/
		return 0;
	}
	else
	{
		/******************/
		/* parsing failed */
		/******************/
		/*****************************************************/
		/* Print an invalid loop function to the output file */
		/*****************************************************/
		ErrorMsg_Log("char *loopFunction(char *input){}\n");
		return -1;
	}
}

