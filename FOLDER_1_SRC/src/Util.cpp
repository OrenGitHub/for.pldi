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
Type *i8_type  = nullptr;
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

/***************************/
/* Get Character (i8) Type */
/***************************/
Type *get_i8_type(LLVMContext &context)
{
	return Type::getInt8Ty(context);
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

/****************/
/* Generic Load */
/****************/
Instruction *LoadIt(Value *v, Instruction *i, const char *name)
{
	auto li = new LoadInst(v,name);
	li->insertBefore(i);
	return li;	
}
/******************************/
/* Load special vars:         */
/*                            */
/* ghost_IVar                 */
/* ghost_SVar                 */
/* global_StatusVar           */
/* global_StrlenVar           */
/*                            */
/******************************/
Instruction *Load_ghost_IVar(Instruction *i)
{
	assert(ghost_IVar && "ghost_IVar is NULL");
	return LoadIt(ghost_IVar,i,"ghost_IVar_Loaded");
}
Instruction *Load_ghost_SVar(Instruction *i)
{
	assert(ghost_SVar && "ghost_SVar is NULL");
	return LoadIt(ghost_SVar,i,"ghost_SVar_Loaded");
}
Instruction *Load_ghost_StatusVar(Instruction *i)
{
	assert(global_StatusVar && "global_StatusVar is NULL");
	return LoadIt(global_StatusVar,i,"global_StatusVar_Loaded");
}
Instruction *Load_global_StrlenVar(Instruction *i)
{
	assert(global_StrlenVar && "global_StrlenVar is NULL");
	return LoadIt(global_StrlenVar,i,"global_StrlenVar_Loaded");
}
/*****************/
/* Generic Store */
/*****************/
void StoreIt(Value *v, Value *dst, Instruction *i)
{
	auto si = new StoreInst(v,dst);
	si->insertBefore(i);
}
/******************************/
/* Store to special vars:     */
/*                            */
/* ghost_IVar                 */
/* ghost_SVar                 */
/* global_StatusVar           */
/* global_StrlenVar           */
/*                            */
/******************************/
void StoreTo_ghost_IVar(      Value *v,Instruction *i){ StoreIt(v,ghost_IVar,      i); }
void StoreTo_ghost_SVar(      Value *v,Instruction *i){ StoreIt(v,ghost_SVar,      i); }
void StoreTo_global_StatusVar(Value *v,Instruction *i){ StoreIt(v,global_StatusVar,i); }
void StoreTo_global_StrlenVar(Value *v,Instruction *i){	StoreIt(v,global_StrlenVar,i); }

Value *cast_to_i32(Value *v,Instruction *i)
{
	auto ext = Instruction::CastOps::ZExt;
	auto ci = CastInst::Create(
		ext,
		v,
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
void Turn_Status_Flag_On(Instruction *i)
{
	/********************/
	/* Actual store ... */
	/********************/
	StoreTo_global_StatusVar(ConstantInt::get(i32_type,1),i);
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
	auto condExtended    = cast_to_i32(cond,i);
	auto condMultiplied  = MulThemUp(condExtended,ConstantInt::get(i32_type,errorCode),i);
	auto addition        = OrThemUp(condMultiplied,statusVarLoaded,i);

	/********************/
	/* Actual store ... */
	/********************/
	StoreTo_global_StatusVar(addition,i);
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

void Instrument_Comparison_Non_Constants(Value *v1, Value *v2, Instruction *i)
{
	Turn_Status_Flag_On_Conditionally(
		NegItUp(
			OrThemUp(
				OrThemUp(
					CheckEquality_Ivar_zero(v1,v2,i),
					CheckEquality_Ivar_zero(v2,v1,i),
					i),
				OrThemUp(
					CheckEquality_Ivar_len(v1,v2,i),
					CheckEquality_Ivar_len(v2,v1,i),
					i),
				i),
			i),
		i);
}

Value *Value_Is_Different_From_Ivar(Value *v,Instruction *i)
{
	auto ne = CmpInst::ICMP_NE;
	auto op = Instruction::OtherOps::ICmp;
	auto ghost_IVar = Load_ghost_SVar(i);
	auto ci = CmpInst::Create(
		op,
		ne,
		v,
		ghost_IVar,
		"value_is_ne_to_ghost_IVar");
	ci->insertBefore(i);
	return ci;
}

Value *Value_Is_Different_From_SVar_Content(Value *v, Instruction *i)
{
	auto ne = CmpInst::ICMP_NE;
	auto op = Instruction::OtherOps::ICmp;
	auto ghost_SVar_Content  = LoadIt(
		Load_ghost_SVar(i),i,
		"ghost_SVar_Content");

	/*********************************************************/
	/* [1] Comparisons come in two "flavours" with i8 or i32 */
	/*     Since ghost_SVar is loaded as i8, the first case  */
	/*     is straight forward, and involves *NO* casting    */
	/*********************************************************/
	if (v->getType() == i8_type)
	{
		auto ci = CmpInst::Create(
			op,
			ne,
			v,
			ghost_SVar_Content,
			"value_is_ne_to_ghost_SVar_content");
		ci->insertBefore(i);
		return ci;
	}
	/*********************************************************/
	/* [2] Comparisons come in two "flavours" with i8 or i32 */
	/*     Since ghost_SVar is loaded as i8, the second case */
	/*     does involve casting to i32.                      */
	/*********************************************************/
	if (v->getType() == i32_type)
	{
		auto ci = CmpInst::Create(
			op,
			ne,
			v,
			cast_to_i32(ghost_SVar_Content,i),
			"value_is_ne_to_ghost_SVar_content");
		ci->insertBefore(i);
		return ci;
	}	
}
