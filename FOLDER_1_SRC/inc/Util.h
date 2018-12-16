#ifndef __UTIL_H__
#define __UTIL_H__

/**********************/
/* LLVM INCLUDE FILES */
/**********************/
#include "llvm/IR/Value.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Instructions.h"

/**************/
/* NAMESPACES */
/**************/
using namespace std;
using namespace llvm;

/********************/
/* GLOBAL VARIABLES */
/********************/
extern Type *i32_type;
extern Type *i8p_type;

/******************************/
/* Get Character Pointer Type */
/******************************/
Type *get_i8p_type(LLVMContext &context);

/********************/
/* Get Int(32) Type */
/********************/
Type *get_i32_type(LLVMContext &context);

/*******************/
/* Is Int(32) Type */
/*******************/
bool is_i32_type(Value *v);

void StoreToStatusVar(  Value *v, Instruction *i);
void StoreToIntGhostVar(Value *v, Instruction *i);
CastInst *ExtendCondition(Value *cond,Instruction *i);

Instruction *OrThemUp( Value *v1,Value *v2, Instruction *i);
Instruction *AndThemUp(Value *v1,Value *v2, Instruction *i);
Instruction *AddThemUp(Value *v1,Value *v2, Instruction *i);
Instruction *SubThemUp(Value *v1,Value *v2, Instruction *i);
Instruction *MulThemUp(Value *v1,Value *v2, Instruction *i);

void Turn_Status_Flag_On_Conditionally(Value *cond,Instruction *i);
Instruction *NegItUp(Value *v,Instruction *i);

Instruction *CheckEquality_Ivar_len( Value *v1,Value *v2,Instruction *i);
Instruction *CheckEquality_Ivar_zero(Value *v1,Value *v2,Instruction *i);


#endif
