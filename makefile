###############
# DIRECTORIES #
###############
BASEDIR = /home/oren/GIT

###############
# DIRECTORIES #
###############
LLVM_DIR         = ${BASEDIR}/llvm-3.8.0
LLVM_SRC_DIR     = ${LLVM_DIR}/llvm
LLVM_BUILD_DIR   = ${LLVM_DIR}/build
LLVM_LIB_DIR     = ${LLVM_BUILD_DIR}/lib
LLVM_INCLUDE_DIR = ${LLVM_SRC_DIR}/include

###############
# DIRECTORIES #
###############
STR_LOOPS_DIR                 = ${BASEDIR}/for.pldi
STR_LOOPS_SRC_DIR             = ${STR_LOOPS_DIR}/FOLDER_1_SRC/src
STR_LOOPS_OBJ_DIR             = ${STR_LOOPS_DIR}/FOLDER_1_SRC/obj
STR_LOOPS_INC_DIR             = ${STR_LOOPS_DIR}/FOLDER_1_SRC/inc
STR_LOOPS_C_EXAMPLES_DIR      = ${STR_LOOPS_EXAMPLES_DIR}/EXAMPLES_C_SRC_DIR
STR_LOOPS_BC_EXAMPLES_DIR     = ${STR_LOOPS_EXAMPLES_DIR}/EXAMPLES_BC_DIR
STR_LOOPS_BC_OPT_EXAMPLES_DIR = ${STR_LOOPS_EXAMPLES_DIR}/EXAMPLES_BC_OPT_DIR
STR_LOOPS_BC_OPT_INSTRUMENTED = ${STR_LOOPS_EXAMPLES_DIR}/EXAMPLES_BCINSTRUMENTED_DIR

#############################
# COMPILER DEFINITION-FLAGS #
#############################
DFLAGS = -D__STDC_CONSTANT_MACROS -D__STDC_LIMIT_MACROS

##########################
# COMPILER INCLUDE-FLAGS #
##########################
IFLAGS =                    \
-I${LLVM_SRC_DIR}/include   \
-I${LLVM_BUILD_DIR}/include \
-I/usr/local/lib

##########################
# COMPILER LIBRARY-FLAGS #
##########################
LFLAGS =                              \
${LLVM_LIB_DIR}/libLLVMIRReader.a     \
${LLVM_LIB_DIR}/libLLVMAsmParser.a    \
${LLVM_LIB_DIR}/libLLVMBitReader.a    \
${LLVM_LIB_DIR}/libLLVMBitWriter.a    \
${LLVM_LIB_DIR}/libLLVMCore.a         \
${LLVM_LIB_DIR}/libLLVMSupport.a      \
-lrt -ldl -ltinfo -lpthread -lz -lm 

################
# SOURCE FILES #
################
STR_LOOPS_SRC_FILES        = $(wildcard  ${STR_LOOPS_SRC_DIR}/*.cpp)
STR_LOOPS_SRC_FILES_NOTDIR = $(notdir    ${STR_LOOPS_SRC_FILES})
STR_LOOPS_OBJ_FILES_NOTDIR = $(patsubst %.cpp, %.cpp.o, ${STR_LOOPS_SRC_FILES_NOTDIR})
STR_LOOPS_OBJ_FILES        = $(addprefix ${STR_LOOPS_OBJ_DIR}/,${STR_LOOPS_OBJ_FILES_NOTDIR})
STR_LOOPS_HEADER_FILES     = $(wildcard  ${STR_LOOPS_INC_DIR}/*.h)

###########################
# RUN ANALYSIS ON BITCODE #
###########################
all: ${STR_LOOPS_DIR}/main

################################################
# [1] compile source file(s) to object file(s) #
################################################
${STR_LOOPS_OBJ_DIR}/%.cpp.o: ${STR_LOOPS_SRC_DIR}/%.cpp ${STR_LOOPS_HEADER_FILES}
	g++ ${DFLAGS} ${IFLAGS} -o $@ -c $<

############################################
# [2] link object files to a runnable main #
############################################
${STR_LOOPS_DIR}/main: ${STR_LOOPS_OBJ_FILES}
	g++ ${STR_LOOPS_OBJ_FILES} -o ${STR_LOOPS_DIR}/main ${LFLAGS}

####################################################
# [3] create bitcode from each example source file #
####################################################
${STR_LOOPS_BC_EXAMPLES_DIR}/%.bc: ${STR_LOOPS_C_EXAMPLES_DIR}/%.c
	clang -emit-llvm -o $@ -c $<

####################################
# [4] opt native passes on bitcode #
####################################
${STR_LOOPS_BC_OPT_EXAMPLES_DIR}/%.bc: ${STR_LOOPS_BC_EXAMPLES_DIR}/%.bc
	opt ${OPT_PASSES} -o $@ $<

#################################
# [5] human readable *.ll files #
#################################
${STR_LOOPS_BC_EXAMPLES_DIR}/%.ll: ${STR_LOOPS_BC_EXAMPLES_DIR}/%.bc
	llvm-dis -o $@ $<

########################################
# [6] instrument the optimized bitcode #
########################################
${STR_LOOPS_BC_OPT_INSTRUMENTED}/%.bc: main ${STR_LOOPS_BC_OPT_EXAMPLES_DIR}/%.bc
	main $< $@

#################################
# [7] human readable *.ll files #
#################################
${STR_LOOPS_BC_OPT_INSTRUMENTED}/%.ll: ${STR_LOOPS_BC_OPT_INSTRUMENTED}/%.bc
	llvm-dis -o $@ $<

