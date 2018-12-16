/**********************/
/* LLVM INCLUDE FILES */
/**********************/
#include "llvm/IR/Value.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Instructions.h"

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
Type *i32_type = nullptr;
Type *i8p_type = nullptr;

/******************************/
/* Get Character Pointer Type */
/******************************/
Type *get_i8p_type(LLVMContext &context)
{
	return Type::getInt8PtrTy(context);
}

/********************/
/* Get Int(32) Type */
/********************/
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

void StoreToStatusVar(Value *v,Instruction *i)
{
	auto si = new StoreInst(v,global_status_var);
	si->insertBefore(i);
}

void StoreToIntGhostVar(Value *v, Instruction *i)
{
	auto si = new StoreInst(v,int_ghost_var);
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

Instruction *AddThemUp(Value *v1,Value *v2,Instruction *i)
{
	auto add = Instruction::BinaryOps::Add;
	auto bo = BinaryOperator::Create(
		add,
		v1,
		v2,
		"addThemUp");
	bo->insertBefore(i);
	return bo;
}

Instruction *SubThemUp(Value *v1,Value *v2,Instruction *i)
{
	auto sub = Instruction::BinaryOps::Sub;
	auto bo = BinaryOperator::Create(
		sub,
		v1,
		v2,
		"subThemUp");
	bo->insertBefore(i);
	return bo;
}

Instruction *MulThemUp(Value *v1,Value *v2,Instruction *i)
{
	auto mul = Instruction::BinaryOps::Mul;
	auto bo = BinaryOperator::Create(
		mul,
		v1,
		v2,
		"mulThemUp");
	bo->insertBefore(i);
	return bo;
}
	
int errorCode = 0;

void Turn_Status_Flag_On_Conditionally(Value *cond,Instruction *i)
{
	auto statusVarLoaded = LoadStatusVar(i);
	auto condExtended    = ExtendCondition(cond,i);
	auto condMultiplied  = MulThemUp(condExtended,ConstantInt::get(i32_type,errorCode),i);
	auto addition        = OrThemUp(condMultiplied,statusVarLoaded,i);
	StoreToStatusVar(addition,i);
}
	
Instruction *OrThemUp(Value *v1,Value *v2,Instruction *i)
{
	auto op = Instruction::BinaryOps::Or;
	auto bo = BinaryOperator::Create(
		op,
		v1,
		v2,
		"orThemUp");
	bo->insertBefore(i);
	return bo;
}
	
Instruction *AndThemUp(Value *v1,Value *v2,Instruction *i)
{
	auto op = Instruction::BinaryOps::And;
	auto bo = BinaryOperator::Create(
		op,
		v1,
		v2,
		"andThemUp");
	bo->insertBefore(i);
	return bo;
}

Instruction *CheckEquality_Ivar_len(Value *v1,Value *v2,Instruction *i)
{
	auto eq = CmpInst::ICMP_EQ;
	auto op = Instruction::OtherOps::ICmp;
	auto ci1 = CmpInst::Create(
		op,
		eq,
		v1,
		LoadGhostIntVar(i),
		"compareToIntGhostVar");
	ci1->insertBefore(i);
	auto ci2 = CmpInst::Create(
		op,
		eq,
		v2,
		LoadStrlenVar(i),
		"compareToStrlen");
	ci2->insertBefore(i);
	return AndThemUp(ci1,ci2,i);
}

Instruction *NegItUp(Value *v,Instruction *i)
{
	assert(v->getType()->isIntegerTy(1));
	auto si = SubThemUp(ConstantInt::get(int1_type,1),v,i);
	return si;
}
	
Instruction *CheckEquality_Ivar_zero(Value *v1,Value *v2,Instruction *i)
{
	auto eq = CmpInst::ICMP_EQ;
	auto zero=ConstantInt::get(i32_type,0);
	auto op = Instruction::OtherOps::ICmp;
	auto ci1 = CmpInst::Create(
		op,
		eq,
		v1,
		LoadGhostIntVar(i),
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

