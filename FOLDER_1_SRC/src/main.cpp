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
#include "llvm/IR/Dominators.h"
#include "llvm/Analysis/LoopInfo.h"

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

/*************************/
/* Instrument Comparison */
/*************************/
void Instrument_Comparison
(
	Value *v1,
	Value *v2,
	bool v1_is_constant,
	bool v2_is_constant
)
{
	if ((v1_is_constant == false) && (v2_is_constant == false))
	{
		//Instrument_Comparison_Non_Constants(v1,v2);
	}
	else if (v1_is_constant == false)
	{
		//Instrument_Comparison_W_Constant(v1);
	}
	else if (v2_is_constant == false)
	{
		//Instrument_Comparison_W_Constant(v2);
	}
	else
	{
		// comparing two constants:
		// should be optimized out
		assert(0);
	}
}

/*************************/
/* Instrument Comparison */
/*************************/
void Instrument_Comparison(Value *v1,Value *v2)
{
	/**************************************/
	/* [1] Check whether v1 is a constant */
	/**************************************/
	bool v1_is_constant = (dyn_cast<Constant>(v1) == nullptr);

	/**************************************/
	/* [2] Check whether v2 is a constant */
	/**************************************/
	bool v2_is_constant = (dyn_cast<Constant>(v2) == nullptr);

	/***************************************/
	/* [3] Handle 4 options of comparisons */
	/***************************************/
	Instrument_Comparison(v1,v2,v1_is_constant,v2_is_constant);
}

/*************************/
/* Instrument Comparison */
/*************************/
void Instrument_Comparison(CmpInst *i)
{
	/*********************************/
	/* [1] Extract the first operand */
	/*********************************/
	auto operand0 = i->getOperand(0);
		
	/**********************************/
	/* [2] Extract the second operand */
	/**********************************/
	auto operand1 = i->getOperand(1);
	
	/**************************************************/
	/* [3] Instrument the comparison between 2 values */
	/**************************************************/
	Instrument_Comparison(operand0,operand1);
}

/**************************/
/* Instrument Comparisons */
/**************************/
void Instrument_Comparisons(Loop *l)
{
	/*************************************/
	/* [1] Iterate over all basic blocks */
	/*************************************/
	for (auto BB = l->block_begin(); BB != l->block_end(); BB++)
	{
		/************************************************************/
		/* [2] Iterate over all the instructions of the basic block */
		/************************************************************/
		for (auto inst = (*BB)->begin(); inst != (*BB)->end(); inst++)
		{
			/*******************************************/
			/* [3] cast iterator to actual instruction */
			/*******************************************/
			Instruction *i = &(*(inst));

			/*****************************/
			/* [4] Instrument Comparison */
			/*****************************/
			if (auto ci = dyn_cast<CmpInst>(i))
			{
				Instrument_Comparison(ci);
			}
		}
	}
}

/**************************/
/* Instrument Comparisons */
/**************************/
void Instrument_Comparisons(Function &f)
{
	/******************************/
	/* [1] Compute Dominator Tree */
	/******************************/
	DominatorTree dt(f);

	/*************************/
	/* [2] Extract Loop Info */
	/*************************/
	LoopInfo li(dt);

	/*************************************/
	/* [3] Iterate over all basic blocks */
	/*************************************/
	for (auto BB = f.begin(); BB != f.end(); BB++)
	{
		/*************************************/
		/* [4] Iterate over all instructions */
		/*************************************/
		for (auto inst = BB->begin(); inst != BB->end(); inst++)
		{
			/*******************************/
			/* [5] Extract the loop itself */
			/*******************************/
			Loop *l = li.getLoopFor(&*BB);

			/*********************/
			/* [6] Is it a loop? */
			/*********************/
			if (l)
			{
				/*****************************************/
				/* [7] Instrument Inner loop comparisons */
				/*****************************************/
				Instrument_Comparisons(l);

				/*************************************/
				/* [8] Assume there is only one loop */
				/*************************************/
				return;
			}
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

	/******************************/
	/* [4] Instrument Comparisons */
	/******************************/
	Instrument_Comparisons(f);

	/***********************************************/
	/* [5] Instrument Pointer & Integer increments */
	/***********************************************/
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

