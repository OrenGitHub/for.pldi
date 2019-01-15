/*************************/
/* GENERAL INCLUDE FILES */
/*************************/
#include <assert.h>

/**********************/
/* LLVM INCLUDE FILES */
/**********************/
#include "llvm/IR/Module.h"
#include "llvm/IR/Value.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/Support/ToolOutputFile.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Bitcode/ReaderWriter.h"
#include "llvm/IR/Dominators.h"
#include "llvm/Analysis/LoopInfo.h"
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
LLVMContext ctx;
Module *M = nullptr;

/********************/
/* GLOBAL VARIABLES */
/********************/
Loop *loop = nullptr;

/********************/
/* GLOBAL VARIABLES */
/********************/
map<Value *,bool> isReadValue;
map<Value *,bool> isPhiValue;

/********************/
/* GLOBAL VARIABLES */
/********************/
int init_I = 0;
int init_S = 0;

/********************/
/* GLOBAL VARIABLES */
/********************/
int inc_I = 1;
int inc_S = 1;

/********************/
/* GLOBAL VARIABLES */
/********************/
Value *ghost_SVar_init_value = nullptr;

/************************************/
/* Initialize myStatus and myStrlen */
/************************************/
void Initialize_Global_Status_Var()
{
	global_StatusVar = M->getGlobalVariable("myStatus");
	assert(global_StatusVar && "global variable myStatus was not found");
}

void Initialize_Global_Strlen_Var()
{
	global_StrlenVar = M->getGlobalVariable("myStrlen");
	assert(global_StrlenVar && "global variable myStrlen was not found");
}

bool BasicBlockBelongsToLoop(BasicBlock *BB, Loop *loop)
{
	for (auto it = loop->block_begin(); it != loop->block_end(); it++)
	{
		if (BB == (*it))
		{
			return true;
		}
	}
	return false;
}

bool Is_Used_Outside_This_loop(Loop *loop, Value *v)
{
	unsigned int n=0;

	/**************************************/
	/* Iterate over *ALL* uses of value v */
	/**************************************/
	for (auto use = v->use_begin(); use != v->use_end(); use++)
	{
		Instruction *i = (Instruction *) &(*(use));

		if (BasicBlockBelongsToLoop(i->getParent(),loop))
		{
			/*******************************************/
			/* Count uses of value v WITHIN the loop l */
			/*******************************************/
			n++;
		}
	}

	/**********************************************/
	/* Sanity check: uses WITHIN loop <= ALL uses */
	/**********************************************/
	assert(n <= v->getNumUses());

	/*****************************************/
	/* Check if: uses WITHIN loop < ALL uses */
	/*****************************************/
	if (n < v->getNumUses())
	{
		/*****************************************************/
		/* found a value that is being used outside the loop */
		/*****************************************************/
		return true;
	}
	return false;
}

bool Is_Type_i8pp(Value *v)
{
	if (v->getType()->isPointerTy())
	{
		PointerType *p1 = (PointerType *) v->getType();
		if (p1->getElementType()->isPointerTy())
		{
			PointerType *p2 = (PointerType *) p1->getElementType();
			if (p2->getElementType()->isIntegerTy(8))
			{
				return true;
			}
		}
	}
	return false;
}

bool I_am_your_svar(Value *v)
{
	return
		Is_Used_Outside_This_loop(loop,v) &&
		Is_Type_i8pp(v);
}

/***************************************/
/* Extract_The_Single_Svar_Of_The_Loop */
/***************************************/
void Extract_The_Single_Svar_Of_The_Loop(Loop *loop)
{
	/*************************************************/
	/* [1] Iterate over all basic blocks of the loop */
	/*************************************************/
	for (auto BB = loop->block_begin();BB != loop->block_end(); BB++)
	{
		for (auto inst = (*BB)->begin(); inst != (*BB)->end(); inst++)
		{
			Instruction *i = &(*(inst));
			
			if (I_am_your_svar(i))
			{
				ghost_SVar_init_value = i;
				return;
			}
			
			if (auto li = dyn_cast<LoadInst>(i))
			{
				if (I_am_your_svar(li->getPointerOperand()))
				{
					ghost_SVar_init_value = li->getPointerOperand();
					return;
				}
			}

			if (auto si = dyn_cast<StoreInst>(i))
			{
				if (I_am_your_svar(si->getPointerOperand()))
				{
					ghost_SVar_init_value = si->getPointerOperand();
					return;
				}
			}
		}
	}
	assert(0 && "svar not found");
}

void GO()
{
	static int i=1;
	BasicBlock *b = nullptr;
	static BasicBlock *orenBB = *(loop->block_begin());
	errs() << "orenBB name: " << orenBB->getName() << "\n";
	
	errs() << "loop address is: " << loop << "\n";
	for (auto BB = loop->block_begin(); BB != loop->block_end(); BB++)
	{
		b = *BB;
		errs() << "Loop " << i << " BB name: " << b->getName() << "\n";
	}
	i++;
}

/**********************************************/
/* Extract_The_Single_Loop_Of_The_String_Func */
/**********************************************/
void Extract_The_Single_Loop_Of_The_String_Func(Function &f,LoopInfo &li)
{
	/*************************************/
	/* [1] Iterate over all basic blocks */
	/*************************************/
	for (auto BB = f.begin(); BB != f.end(); BB++)
	{
		/*************************************/
		/* [2] Iterate over all instructions */
		/*************************************/
		for (auto inst = BB->begin(); inst != BB->end(); inst++)
		{
			/*******************************/
			/* [3] Extract the loop itself */
			/*******************************/
			loop = li.getLoopFor(&*BB);

			/*********************/
			/* [4] Is it a loop? */
			/*********************/
			if (loop)
			{
				return;
			}
		}
	}
	assert(0 && "no loops found in string func");
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

/**************************************/
/* Allocate And Initialize Ghost Vars */
/**************************************/
void Allocate_Ghost_Vars(Function &f)
{
	/*******************************************************/
	/* [1] Take the very first basic block of the function */
	/*******************************************************/
	auto BB = f.begin();

	/*********************************************/
	/* [2] Take the very first instruction of BB */
	/*********************************************/
	auto inst = BB->begin();

	/*******************************************/
	/* [3] cast iterator to actual instruction */
	/*******************************************/
	Instruction *i = &(*(inst));

	/******************************************************/
	/* [4] Actual Allocation of ghost_IVar and ghost_SVar */
	/******************************************************/
	ghost_IVar = AllocateVarHere(i,i32_type,"ghost_IVar",4);
	ghost_SVar = AllocateVarHere(i,i8p_type,"ghost_SVar",8);
}

/**************************************/
/* Allocate And Initialize Ghost Vars */
/**************************************/
void Initialize_Ghost_Vars(Function &f, int init_I, int init_S)
{
	/*******************************************************/
	/* [1] Take the very first basic block of the function */
	/*******************************************************/
	auto BB = f.begin();

	/********************************************/
	/* [2] Take the very last instruction of BB */
	/********************************************/
	Instruction *i = (&(*BB))->getTerminator();

	/*****************************/
	/* [3] Initialize ghost_IVar */
	/*****************************/
	Initialize_Ghost_IVar(init_I,i);

	/***********************************/
	/* [4] Find *THE* svar of the loop */
	/***********************************/
	Extract_The_Single_Svar_Of_The_Loop(loop);

	/*****************************/
	/* [5] Initialize ghost_SVar */
	/*****************************/
	Initialize_Ghost_SVar(init_S,i);
}

/**************************************/
/* Allocate And Initialize Ghost Vars */
/**************************************/
void Allocate_And_Initialize_Ghost_Vars(Function &f, int init_I, int init_S)
{
	/******************************************/
	/* [1] Allocate ghost_IVar and ghost_SVar */
	/******************************************/
	Allocate_Ghost_Vars(f);

	/********************************************/
	/* [2] Initialize ghost_IVar and ghost_SVar */
	/********************************************/
	Initialize_Ghost_Vars(f,init_I,init_S);
}

/*************************/
/* Instrument Comparison */
/*************************/
void Instrument_Comparison_W_Constant
(
	Value *v,
	Instruction *i,
	int constant
)
{
	if (isReadValue[v])
	{
		/**********************************************/
		/* [1] If v is a read value, then it must     */
		/*     be equal to [ ghost_SVar ]. Otherwise, */
		/*     it is an error. That is, we turn the   */
		/*     status flag on.                        */
		/**********************************************/
		Turn_Status_Flag_On_Conditionally(
			Value_Is_Different_From_SVar_Content(v,i),
			i);
	}
	else if (is_i32_type(v) && (constant == 0))
	{
		/*************************************************/
		/* [2] If v is *NOT* a read value, then the      */
		/*     only other option is that v is equal to   */
		/*     ghost_IVar, and the constant that is      */
		/*     being compared to is zero (0). Otherwise, */
		/*     we turn the status flag on.               */
		/*************************************************/
		Turn_Status_Flag_On_Conditionally(
			Value_Is_Different_From_Ivar(v,i),
			i);
	}
}

/*************************/
/* Instrument Comparison */
/*************************/
void Instrument_Comparison
(
	Value *v1,
	Value *v2,
	ConstantInt *v1_as_const,
	ConstantInt *v2_as_const,
	Instruction *i
)
{
	int constant;
	
	if ((v1_as_const == nullptr) && (v2_as_const == nullptr))
	{
		Instrument_Comparison_Non_Constants(v1,v2,i);
	}
	else if (v1_as_const == nullptr)
	{
		constant = v2_as_const->getSExtValue();
		Instrument_Comparison_W_Constant(v1,i,constant);
	}
	else if (v2_as_const == nullptr)
	{
		constant = v1_as_const->getSExtValue();
		Instrument_Comparison_W_Constant(v2,i,constant);
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
void Instrument_Comparison
(
	Value *v1,
	Value *v2,
	Instruction *i
)
{
	Instrument_Comparison(
		v1,
		v2,
		dyn_cast<ConstantInt>(v1),
		dyn_cast<ConstantInt>(v2),
		i);
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
	Instrument_Comparison(operand0,operand1,i);
}

/*******************************/
/* Instrument Loop Comparisons */
/*******************************/
void Instrument_Comparisons(Loop *loop)
{
	/*************************************/
	/* [1] Iterate over all basic blocks */
	/*************************************/
	for (auto BB = loop->block_begin(); BB != loop->block_end(); BB++)
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

/*******************/
/* Instrument Read */
/*******************/
void Instrument_Assign(Value *v1, Value *v2)
{
	if (isReadValue[v1])
	{
		isReadValue[v2] = true;
	}
}

/**********************/
/* Instrument Assigns */
/**********************/
void Instrument_Assigns(const Loop *loop)
{
	/*************************************/
	/* [1] Iterate over all basic blocks */
	/*************************************/
	for (auto BB = loop->block_begin(); BB != loop->block_end(); BB++)
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
			/* [4] Instrument Assignment */
			/*****************************/
			if (auto si = dyn_cast<SExtInst> (i)) { Instrument_Assign(si->getOperand(0),si); }
			if (auto zi = dyn_cast<ZExtInst> (i)) { Instrument_Assign(zi->getOperand(0),zi); }
			if (auto ti = dyn_cast<TruncInst>(i)) { Instrument_Assign(ti->getOperand(0),ti); }
		}
	}
}

/*******************/
/* Instrument Read */
/*******************/
void Instrument_Read(LoadInst *i)
{
	if (i->getType()->isIntegerTy(8))
	{
		isReadValue[i] = true;
	}		
}

/********************/
/* Instrument Loads */
/********************/
void Instrument_Reads(Loop *loop)
{
	/*************************************/
	/* [1] Iterate over all basic blocks */
	/*************************************/
	for (auto BB = loop->block_begin(); BB != loop->block_end(); BB++)
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

			/***********************/
			/* [4] Instrument Load */
			/***********************/
			if (auto li = dyn_cast<LoadInst>(i))
			{
				Instrument_Read(li);
			}
		}
	}
}

/**********************************/
/* Function Calls Other Functions */
/**********************************/
bool Function_Calls_Other_Functions(Function &f)
{
	/*************************************/
	/* [1] Iterate over all basic blocks */
	/*************************************/
	for (auto BB = f.begin(); BB != f.end(); BB++)
	{
		/*************************************/
		/* [2] Iterate over all instructions */
		/*************************************/
		for (auto inst = BB->begin(); inst != BB->end(); inst++)
		{
			/*********************************************/
			/* [3] Cast from iterator to (Instruction *) */
			/*********************************************/
			Instruction *i = &(*(inst));

			/***********************************************/
			/* [4] Check if function calls other functions */
			/***********************************************/
			if (dyn_cast<CallInst>(i))
			{
				/***********************************************/
				/* [5] if it does, let it turn the status flag */
				/*     on, so we know it's not valid           */
				/***********************************************/
				Turn_Status_Flag_On(i);
				return 1;
			}
		}
	}
}

/**************************/
/* Handle String Function */
/**************************/
void HandleStringFunc(Function &f)
{
	/***********************/
	/* [0] Initializations */
	/***********************/
	i1_type  = get_i1_type( ctx);
	i8_type  = get_i8_type( ctx);
	i32_type = get_i32_type(ctx);
	i8p_type = get_i8p_type(ctx);

	/**********************************************/
	/* [1] Initialize global status & strlen vars */
	/**********************************************/
	Initialize_Global_Status_Var();
	Initialize_Global_Strlen_Var();

	/**********************************************************/
	/* [2] Exclude string functions that call other functions */
	/**********************************************************/
	if (Function_Calls_Other_Functions(f)) { return; }

	/**************************************************/
	/* [3] Compute Dominator Tree & Extract Loop Info */
	/**************************************************/
	DominatorTree dt(f); LoopInfo li(dt);

	/****************************************************/
	/* [4] Extract the *SINGLE* loop of the String Func */
	/****************************************************/
	Extract_The_Single_Loop_Of_The_String_Func(f,li);

	/*********************************************************/
	/* [5] Allocate and Initialize ghost_IVar and ghost_SVar */
	/*********************************************************/
	Allocate_And_Initialize_Ghost_Vars(f,init_I,init_S);

	/***********************************************/
	/* [6] Instrument Loads, Reads and Comparisons */
	/***********************************************/
	Instrument_Reads(loop);
	Instrument_Assigns(loop);
	Instrument_Comparisons(loop);

	/***********************************************/
	/* [7] Instrument Pointer & Integer increments */
	/***********************************************/
	Update_Ghost_IVar(loop,inc_I);
	Update_Ghost_SVar(loop,inc_S);
}

/*****************/
/* Handle Module */
/*****************/
void HandleModule(Module *M)
{
	for (auto f = M->begin(); f != M->end(); f++)
	{
		if (f->getName().str() == "loopFunction")
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

