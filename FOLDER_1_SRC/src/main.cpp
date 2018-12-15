/**********************/
/* LLVM INCLUDE FILES */
/**********************/
#include "llvm/IR/Module.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/Support/ToolOutputFile.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Bitcode/ReaderWriter.h"

/**************/
/* NAMESPACES */
/**************/
using namespace std;
using namespace llvm;

/********************/
/* GLOBAL VARIABLES */
/********************/
Value *ghost_IVar = nullptr;
Value *ghost_SVar = nullptr;

/********************/
/* GLOBAL VARIABLES */
/********************/
LLVMContext ctx;

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

/*********************/
/* Allocate Var Here */
/*********************/
Value *AllocateVarHere
(
	Instruction *i,
	Type *type,
	const string &name,
	const unsigned int alignment
)
{
	/************************************/
	/* [1] Create an Alloca instruction */
	/************************************/
	auto ai = new AllocaInst(type,0,name);

	/******************************************/
	/* [2] Set alignment to be extra cautious */
	/******************************************/
	ai->setAlignment(alignment);

	/******************************************/
	/* [3] Insert Alloca instruction before i */
	/******************************************/
	ai->insertBefore(i);

	/******************************************/
	/* [4] return ai for possible assignments */
	/******************************************/
	return ai;
}

/***********************/
/* Allocate Ghost Vars */
/***********************/
void Allocate_Ghost_Vars(Function &f)
{
	/*************************************/
	/* [1] Iterate over all basic blocks */
	/*************************************/
	for (auto BB = f.begin(); BB != f.end(); BB++)
	{
		/************************************************************/
		/* [2] Iterate over all the instructions of the basic block */
		/************************************************************/
		for (auto inst = BB->begin(); inst != BB->end(); inst++)
		{
			/*******************************************/
			/* [3] cast iterator to actual instruction */
			/*******************************************/
			Instruction *i = &(*(inst));

			/************************/
			/* [4a] Get an i32 type */
			/* [4b] Get an i8* type */
			/************************/
			auto i32_type = get_i32_type(ctx);
			auto i8p_type = get_i8p_type(ctx);

			/******************************************************/
			/* [5] Actual Allocation of ghost_IVar and ghost_SVar */
			/******************************************************/
			ghost_IVar = AllocateVarHere(i,i32_type,"ghost_IVar",4);
			ghost_SVar = AllocateVarHere(i,i8p_type,"ghost_SVar",8);

			/************************************************************/
			/* [6] Ha ha ... fancy nested loop ... all we actually need */
			/*     is the very first instruction of the function        */
			/************************************************************/
			return;
		}
	}
}

/**************************/
/* Handle String Function */
/**************************/
void HandleStringFunc(Function &f)
{
	/******************************/
	/* [1] Print Handled Function */
	/******************************/
	errs() << f.getName() << "\n";

	/******************************************/
	/* [2] Allocate ghost_IVar and ghost_SVar */
	/******************************************/
	Allocate_Ghost_Vars(f);

	/********************************************/
	/* [3] Initialize ghost_IVar and ghost_SVar */
	/********************************************/
	// Initialize_Ghost_Vars(f,Ioffset,Soffset);
}

/*****************/
/* Handle Module */
/*****************/
void HandleModule(Module *M)
{
	for (auto f = M->begin(); f != M->end(); f++)
	{
		if (f->getName().str() != "main")
		{
			HandleStringFunc(*f);
		}
	}
}

/********/
/* main */
/********/
int main(int argc, char **argv)
{
	error_code ec;
	SMDiagnostic Err;
	Module *M = nullptr;
	unique_ptr<Module> module;

	/*****************************************/
	/* [1] Read LLVM bitcode from input file */
	/*****************************************/
	module = parseIRFile(argv[1],Err,ctx);
	if (!module) { return 0; }
	else { M = module.get(); }

	/*******************************/
	/* [2] Instrument LLVM bitcode */
	/*******************************/
	HandleModule(M);

	/******************************************************/
	/* [3] Write instrumented LLVM bitcode to output file */
	/******************************************************/
	tool_output_file result(argv[2],ec,sys::fs::F_None);
	WriteBitcodeToFile(M,result.os());
	result.keep();

	/******************/
	/* [4] Return ... */
	/******************/
    return 0;
}

