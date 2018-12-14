/**********************/
/* LLVM INCLUDE FILES */
/**********************/
#include "llvm/IR/Module.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Instructions.h"

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

/******************************/
/* Get Character Pointer Type */
/******************************/
PointerType *getCharPtrType(Instruction *i)
{
	return Type::getInt8PtrTy(i->getParent()->getParent()->getContext());
}

/*********************/
/* Allocate Var Here */
/*********************/
Value *AllocateVarHere(Instruction *i,const string &name)
{
	/************************************/
	/* [1] Get a character pointer type */
	/************************************/
	auto char_ptr_type  = getCharPtrType(i);

	/************************************/
	/* [2] Create an Alloca instruction */
	/************************************/
	auto ai = new AllocaInst(char_ptr_type,0,name);

	/******************************************/
	/* [3] Set alignment to be extra cautious */
	/******************************************/
	ai->setAlignment(8);

	/******************************************/
	/* [4] Insert Alloca instruction before i */
	/******************************************/
	ai->insertBefore(i);

	/******************************************/
	/* [5] return ai for possible assignments */
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

			/******************************************************/
			/* [4] Actual Allocation of ghost_IVar and ghost_SVar */
			/******************************************************/
			ghost_IVar = AllocateVarHere(i,"ghost_IVar");
			ghost_SVar = AllocateVarHere(i,"ghost_SVar");

			/************************************************************/
			/* [5] Ha ha ... fancy nested loop ... all we actually need */
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
	/****************/
	/* LLVM Context */
	/****************/
	LLVMContext ctx;

	/********************/
	/* LLVM Diagnostics */
	/********************/
	SMDiagnostic Err;

	/***************/
	/* LLVM Module */
	/***************/
	unique_ptr<Module> M = parseIRFile(argv[1],Err,ctx);

	/*****************/
	/* Handle Module */
	/*****************/
	if (M) HandleModule(M.get());

	/**************/
	/* Return ... */
	/**************/
    return 0;
}

