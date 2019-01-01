/**********************/
/* LLVM INCLUDE FILES */
/**********************/
#include "llvm/IR/Value.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Constants.h"

/*************************/
/* PROJECT INCLUDE FILES */
/*************************/
#include "Util.h"

/**************/
/* NAMESPACES */
/**************/
using namespace std;
using namespace llvm;

/********************/
/* GLOBAL VARIABLES */
/********************/
Type *i1_type  = nullptr;
Type *i32_type = nullptr;
Type *i8p_type = nullptr;

/********************/
/* GLOBAL VARIABLES */
/********************/
Value *ghost_IVar = nullptr;
Value *ghost_SVar = nullptr;

/********************/
/* GLOBAL VARIABLES */
/********************/
Value *global_StatusVar = nullptr;
Value *global_StrlenVar = nullptr;

/***************************/
/* STATIC GLOBAL VARIABLES */
/***************************/
static int errorCode = 0;

/*************************/
/* Get Boolean (i1) Type */
/*************************/
Type *get_i1_type(LLVMContext &context)
{
	return Type::getInt1Ty(context);
}

/************************************/
/* Get Character Pointer (i8*) Type */
/************************************/
Type *get_i8p_type(LLVMContext &context)
{
	return Type::getInt8PtrTy(context);
}

/**********************/
/* Get Int (i32) Type */
/**********************/
Type *get_i32_type(LLVMContext &context)
{
	return Type::getInt32Ty(context);
}

/*******************/
/* Is Int(32) Type */
/*******************/
bool is_i32_type(Value *v)
{
	return v->getType()->isIntegerTy(32);
}

/******************************/
/* Load special vars:         */
/*                            */
/* ghost_IVar                 */
/* ghost_SVar                 */
/* ghost_StatusVar            */
/* global_StrlenVar            */
/*                            */
/******************************/
Instruction *Load_ghost_IVar(Instruction *i)
{
	auto li = new LoadInst(ghost_IVar,"ghost_IVar_Loaded");
	li->insertBefore(i);
	return li;
}
Instruction *Load_ghost_SVar(Instruction *i)
{
	auto li = new LoadInst(ghost_SVar,"ghost_SVar_Loaded");
	li->insertBefore(i);
	return li;
}
LoadInst *Load_ghost_StatusVar(Instruction *i)
{
	auto li = new LoadInst(global_StatusVar,"global_StatusVar_Loaded");
	li->insertBefore(i);
	return li;
}
Instruction *Load_global_StrlenVar(Instruction *i)
{
	auto li = new LoadInst(global_StrlenVar,"global_StrlenVar_Loaded");
	li->insertBefore(i);
	return li;
}
/******************************/
/* Store to special vars:     */
/*                            */
/* ghost_IVar                 */
/* ghost_SVar                 */
/* ghost_StatusVar            */
/* global_StrlenVar            */
/*                            */
/******************************/
void StoreTo_ghost_IVar(Value *v, Instruction *i)
{
	auto si = new StoreInst(v,ghost_IVar);
	si->insertBefore(i);
}
void StoreTo_ghost_SVar(Value *v, Instruction *i)
{
	auto si = new StoreInst(v,ghost_SVar);
	si->insertBefore(i);
}
void StoreTo_ghost_StatusVar(Value *v,Instruction *i)
{
	auto si = new StoreInst(v,global_StatusVar);
	si->insertBefore(i);
}
void StoreTo_global_StrlenVar(Value *v,Instruction *i)
{
	auto si = new StoreInst(v,global_StrlenVar);
	si->insertBefore(i);
}

CastInst *ExtendCondition(Value *cond,Instruction *i)
{
	auto ext = Instruction::CastOps::ZExt;
	auto ci = CastInst::Create(
		ext,
		cond,
		i32_type,
		"zero_extended");
	ci->insertBefore(i);
	return ci;
}

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
Instruction *AddThemUp(Value *v1,Value *v2,Instruction *i)
{
	auto add = Instruction::BinaryOps::Add;
	auto bo = BinaryOperator::Create(add,v1,v2,"addThemUp");
	bo->insertBefore(i);
	return bo;
}
Instruction *SubThemUp(Value *v1,Value *v2,Instruction *i)
{
	auto sub = Instruction::BinaryOps::Sub;
	auto bo = BinaryOperator::Create(sub,v1,v2,"subThemUp");
	bo->insertBefore(i);
	return bo;
}
Instruction *MulThemUp(Value *v1,Value *v2,Instruction *i)
{
	auto mul = Instruction::BinaryOps::Mul;
	auto bo = BinaryOperator::Create(mul,v1,v2,"mulThemUp");
	bo->insertBefore(i);
	return bo;
}	
Instruction *OrThemUp(Value *v1,Value *v2,Instruction *i)
{
	auto op = Instruction::BinaryOps::Or;
	auto bo = BinaryOperator::Create(op,v1,v2,"orThemUp");
	bo->insertBefore(i);
	return bo;
}
Instruction *AndThemUp(Value *v1,Value *v2,Instruction *i)
{
	auto op = Instruction::BinaryOps::And;
	auto bo = BinaryOperator::Create(op,v1,v2,"andThemUp");
	bo->insertBefore(i);
	return bo;
}

Instruction *NegItUp(Value *v,Instruction *i)
{
	assert(v->getType()->isIntegerTy(1));
	auto si = SubThemUp(ConstantInt::get(i1_type,1),v,i);
	return si;
}
/********************************************************/
/* Turn Status flag one conditionally (with error code) */
/********************************************************/
void Turn_Status_Flag_On_Conditionally(Value *cond,Instruction *i)
{
	/***************/
	/* Helper Vars */
	/***************/
	auto statusVarLoaded = Load_ghost_StatusVar(i);
	auto condExtended    = ExtendCondition(cond,i);
	auto condMultiplied  = MulThemUp(condExtended,ConstantInt::get(i32_type,errorCode),i);
	auto addition        = OrThemUp(condMultiplied,statusVarLoaded,i);

	/********************/
	/* Actual store ... */
	/********************/
	StoreTo_ghost_StatusVar(addition,i);
}
/***************************************************/
/* Check Equality and make sure it satisfies:      */
/*                                                 */
/* LHS == ghost_IVar                               */
/* RHS == global_StrlenVar                          */
/*                                                 */
/***************************************************/
Instruction *CheckEquality_Ivar_len(Value *v1,Value *v2,Instruction *i)
{
	auto eq = CmpInst::ICMP_EQ;
	auto op = Instruction::OtherOps::ICmp;
	auto ci1 = CmpInst::Create(
		op,
		eq,
		v1,
		Load_ghost_IVar(i),
		"compareTo_ghost_IVar");
	ci1->insertBefore(i);
	auto ci2 = CmpInst::Create(
		op,
		eq,
		v2,
		Load_global_StrlenVar(i),
		"compareTo_Strlen");
	ci2->insertBefore(i);
	return AndThemUp(ci1,ci2,i);
}
/***************************************************/
/* Check Equality and make sure it satisfies:      */
/*                                                 */
/* LHS == ghost_IVar                               */
/* RHS == CONST(0)                                 */
/*                                                 */
/***************************************************/
Instruction *CheckEquality_Ivar_zero(Value *v1,Value *v2,Instruction *i)
{
	auto eq = CmpInst::ICMP_EQ;
	auto zero=ConstantInt::get(i32_type,0);
	auto op = Instruction::OtherOps::ICmp;
	auto ci1 = CmpInst::Create(
		op,
		eq,
		v1,
		Load_ghost_IVar(i),
		"compareToIVar");
	ci1->insertBefore(i);
	auto ci2 = CmpInst::Create(
		op,
		eq,
		v2,
		zero,
		"compareToStrlen");
	ci2->insertBefore(i);
	return AndThemUp(ci1,ci2,i);		
}
void Update_Ghost_IVar(Function &f, int inc_I)
{
}

void Update_Ghost_SVar(Function &f, int inc_S)
{
}

void Initialize_Ghost_IVar(Function &f,int init_I)
{
	
}
void Initialize_Ghost_SVar(Function &f,int init_S)
{
	
}

