#ifndef __UTIL_H__
#define __UTIL_H__

/**********************/
/* LLVM INCLUDE FILES */
/**********************/
#include "llvm/IR/Value.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Analysis/LoopInfo.h"

/**************/
/* NAMESPACES */
/**************/
using namespace std;
using namespace llvm;

/********************/
/* GLOBAL VARIABLES */
/********************/
extern Type *i1_type;
extern Type *i8_type;
extern Type *i32_type;
extern Type *i8p_type;

/********************/
/* GLOBAL VARIABLES */
/********************/
extern Value *ghost_IVar;
extern Value *ghost_SVar;

/********************/
/* GLOBAL VARIABLES */
/********************/
extern Value *global_StatusVar;
extern Value *global_StrlenVar;

/********************/
/* GLOBAL VARIABLES */
/********************/
extern Value *ghost_SVar_init_value;

/*************************/
/* Get Boolean (i1) Type */
/*************************/
Type *get_i1_type(LLVMContext &context);

/************************************/
/* Get Character Pointer (i8*) Type */
/************************************/
Type *get_i8p_type(LLVMContext &context);

/***************************/
/* Get Character (i8) Type */
/***************************/
Type *get_i8_type(LLVMContext &context);

/**********************/
/* Get Int (i32) Type */
/**********************/
Type *get_i32_type(LLVMContext &context);

/*******************/
/* Is Int(32) Type */
/*******************/
bool is_i32_type(Value *v);

/******************************/
/* Load special vars:         */
/*                            */
/* ghost_IVar                 */
/* ghost_SVar                 */
/* global_StatusVar           */
/* global_StrlenVar           */
/*                            */
/******************************/
Instruction *Load_ghost_IVar(      Instruction *i);
Instruction *Load_ghost_SVar(      Instruction *i);
Instruction *Load_ghost_StatusVar( Instruction *i);
Instruction *Load_global_StrlenVar(Instruction *i);

/******************************/
/* Store to special vars:     */
/*                            */
/* ghost_IVar                 */
/* ghost_SVar                 */
/* global_StatusVar           */
/* global_StrlenVar           */
/*                            */
/******************************/
void StoreTo_ghost_IVar(        Value *v, Instruction *i);
void StoreTo_ghost_SVar(        Value *v, Instruction *i);
void StoreTo_global_StatusVar(  Value *v, Instruction *i);
void StoreTo_global_StrlenVar(  Value *v, Instruction *i);

CastInst *ExtendCondition(Value *cond,Instruction *i);

/*************************/
/* The BINOP operations: */
/*                       */
/* Add                   */
/* Sub                   */
/* Mul                   */
/* And                   */
/* Or                    */
/*                       */
/*************************/
Instruction *AddThemUp(Value *v1,Value *v2, Instruction *i);
Instruction *SubThemUp(Value *v1,Value *v2, Instruction *i);
Instruction *MulThemUp(Value *v1,Value *v2, Instruction *i);
Instruction *AndThemUp(Value *v1,Value *v2, Instruction *i);
Instruction *OrThemUp( Value *v1,Value *v2, Instruction *i);

void Turn_Status_Flag_On(Instruction *i);
void Turn_Status_Flag_On_Conditionally(Value *cond,Instruction *i);
Instruction *NegItUp(Value *v,Instruction *i);

Instruction *CheckEquality_Ivar_len( Value *v1,Value *v2,Instruction *i);
Instruction *CheckEquality_Ivar_zero(Value *v1,Value *v2,Instruction *i);

/*************************/
/* Initialize_Ghost_IVar */
/* Initialize_Ghost_SVar */
/*************************/
void Initialize_Ghost_IVar(int init_I,Instruction *i);
void Initialize_Ghost_SVar(int init_S,Instruction *i);

/*********************/
/* Update_Ghost_IVar */
/* Update_Ghost_IVar */
/*********************/
void Update_Ghost_IVar(Loop *loop,int inc_I);
void Update_Ghost_SVar(Loop *loop,int inc_S);

void Instrument_Comparison_Non_Constants(Value *v1, Value *v2, Instruction *i);

Value *Value_Is_Different_From_Ivar(        Value *v, Instruction *i);
Value *Value_Is_Different_From_SVar_Content(Value *v, Instruction *i);

#endif
