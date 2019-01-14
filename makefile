###############
# DIRECTORIES #
###############
BASEDIR = ${HOME}/GIT

###############
# DIRECTORIES #
###############
LLVM_DIR         = ${BASEDIR}/llvm-3.8.0
LLVM_SRC_DIR     = ${LLVM_DIR}/llvm
LLVM_BUILD_DIR   = ${LLVM_DIR}/build-debug
LLVM_LIB_DIR     = ${LLVM_BUILD_DIR}/lib
LLVM_INCLUDE_DIR = ${LLVM_SRC_DIR}/include

###############
# DIRECTORIES #
###############
STR_LOOPS_DIR                 = ${BASEDIR}/for.pldi
STR_LOOPS_SRC_DIR             = ${STR_LOOPS_DIR}/FOLDER_1_SRC/src
STR_LOOPS_OBJ_DIR             = ${STR_LOOPS_DIR}/FOLDER_1_SRC/obj
STR_LOOPS_INC_DIR             = ${STR_LOOPS_DIR}/FOLDER_1_SRC/inc
STR_LOOPS_EXAMPLES_DIR        = ${STR_LOOPS_DIR}/FOLDER_0_EXAMPLES
STR_LOOPS_C_EXAMPLES_DIR      = ${STR_LOOPS_EXAMPLES_DIR}/EXAMPLES_C_SRC_DIR
STR_LOOPS_BC_EXAMPLES_DIR     = ${STR_LOOPS_EXAMPLES_DIR}/EXAMPLES_BC_DIR
STR_LOOPS_LL_EXAMPLES_DIR     = ${STR_LOOPS_EXAMPLES_DIR}/EXAMPLES_LL_DIR
STR_LOOPS_BC_OPT_EXAMPLES_DIR = ${STR_LOOPS_EXAMPLES_DIR}/EXAMPLES_BC_OPT_DIR
STR_LOOPS_LL_OPT_EXAMPLES_DIR = ${STR_LOOPS_EXAMPLES_DIR}/EXAMPLES_LL_OPT_DIR
STR_LOOPS_BC_OPT_INSTRUMENTED = ${STR_LOOPS_EXAMPLES_DIR}/EXAMPLES_BC_OPT_INSTRUMENTED_DIR
STR_LOOPS_LL_OPT_INSTRUMENTED = ${STR_LOOPS_EXAMPLES_DIR}/EXAMPLES_LL_OPT_INSTRUMENTED_DIR
STR_LOOPS_STATUS_DIR          = ${STR_LOOPS_EXAMPLES_DIR}/EXAMPLES_STATUS_DIR
STR_LOOPS_SCRIPTS_DIR         = ${STR_LOOPS_DIR}/FOLDER_2_SCRIPTS

###############
# DIRECTORIES #
###############
KLEE_OUTPUT_DIR = ${STR_LOOPS_EXAMPLES_DIR}/EXAMPLES_KLEE_OUTPUT_DIR

#########
# FILES #
#########
STATUS_FILE = ${STR_LOOPS_DIR}/status.csv

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
-I${STR_LOOPS_INC_DIR}      \
-I/usr/local/lib

##########################
# COMPILER LIBRARY-FLAGS #
##########################
LFLAGS =                            \
${LLVM_LIB_DIR}/libLLVMIRReader.a   \
${LLVM_LIB_DIR}/libLLVMAsmParser.a  \
${LLVM_LIB_DIR}/libLLVMAnalysis.a   \
${LLVM_LIB_DIR}/libLLVMBitReader.a  \
${LLVM_LIB_DIR}/libLLVMBitWriter.a  \
${LLVM_LIB_DIR}/libLLVMCore.a       \
${LLVM_LIB_DIR}/libLLVMSupport.a    \
-lrt -ldl -ltinfo -lpthread -lz -lm 

########
# KLEE #
########
KLEE_DIR     = ${BASEDIR}/klee/vanilla.klee
KLEE_SRC_DIR = ${KLEE_DIR}/klee
KLEE_BIN_DIR = ${KLEE_DIR}/build-with-clang-3.8.0/bin
KLEE_INC_DIR = ${KLEE_SRC_DIR}/include
KLEE         = ${KLEE_BIN_DIR}/klee

#########
# OTHER #
#########
ASSERTION_FAIL_PATTERN = "ASSERTION FAIL"
KLEE_DONE_PATTERN = "KLEE: done"

#########
# CLANG #
#########
CLANG_FLAGS = -w -c -emit-llvm -O0 -I${KLEE_INC_DIR}
OPT_PASSES = -instnamer

################
# SOURCE FILES #
################
STR_LOOPS_SRC_FILES            = $(wildcard  ${STR_LOOPS_SRC_DIR}/*.cpp)
STR_LOOPS_SRC_FILES_NOTDIR     = $(notdir    ${STR_LOOPS_SRC_FILES})
STR_LOOPS_OBJ_FILES_NOTDIR     = $(patsubst %.cpp, %.cpp.o, ${STR_LOOPS_SRC_FILES_NOTDIR})
STR_LOOPS_OBJ_FILES            = $(addprefix ${STR_LOOPS_OBJ_DIR}/,${STR_LOOPS_OBJ_FILES_NOTDIR})
STR_LOOPS_HEADER_FILES         = $(wildcard  ${STR_LOOPS_INC_DIR}/*.h)
STR_LOOPS_EXAMPLE_FILES        = $(wildcard  ${STR_LOOPS_C_EXAMPLES_DIR}/*.c)
STR_LOOPS_EXAMPLE_FILES_NOTDIR = $(notdir    ${STR_LOOPS_EXAMPLE_FILES})
STR_LOOPS_STATUS__FILES_NOTDIR = $(patsubst %.c, %.status, ${STR_LOOPS_EXAMPLE_FILES_NOTDIR})
STR_LOOPS_STATUS__FILES        = $(addprefix ${STR_LOOPS_STATUS_DIR}/,${STR_LOOPS_STATUS__FILES_NOTDIR})

#################
# BITCODE FILES #
#################
STR_LOOPS_BC_OPT_INSTRUMENTED_FILES = $(wildcard ${STR_LOOPS_BC_OPT_INSTRUMENTED}/*.bc)

##########################################
# DONT TOUCH MY *.ll GENERATED FILES !!! #
##########################################
.SECONDARY:

######################
# [0] default target #
######################
.DEFAULT_GOAL := ${STATUS_FILE}

############################################################
# [1] compile application source file(s) to object file(s) #
############################################################
${STR_LOOPS_OBJ_DIR}/%.cpp.o: ${STR_LOOPS_SRC_DIR}/%.cpp ${STR_LOOPS_HEADER_FILES}
	@echo "[ 0 ] Compiling Application File: $<"
	@g++ -g -O0 ${DFLAGS} ${IFLAGS} -o $@ -c $<

##########################################################
# [2] link object files to a runnable application (main) #
##########################################################
${STR_LOOPS_DIR}/main: ${STR_LOOPS_OBJ_FILES}
	@echo "[ 0 ] Linking Application File : ${STR_LOOPS_DIR}/main"
	@g++ ${STR_LOOPS_OBJ_FILES} -o ${STR_LOOPS_DIR}/main ${LFLAGS}

####################################################
# [3] create bitcode from each example source file #
####################################################
${STR_LOOPS_BC_EXAMPLES_DIR}/%.bc: \
${STR_LOOPS_C_EXAMPLES_DIR}/%.c    \
${STR_LOOPS_DIR}/main
	@echo "[ 1 ] Generating bitcode   from: $<"
	@clang ${CLANG_FLAGS} $< -o $@

######################################################
# [4] create human readable *.ll file from *.bc file #
######################################################
${STR_LOOPS_LL_EXAMPLES_DIR}/%.ll: \
${STR_LOOPS_BC_EXAMPLES_DIR}/%.bc
	@echo "[ 2 ] Generating *.ll file from: $<"
	@llvm-dis $< -o $@

####################################
# [5] opt native passes on bitcode #
####################################
${STR_LOOPS_BC_OPT_EXAMPLES_DIR}/%.bc: \
${STR_LOOPS_BC_EXAMPLES_DIR}/%.bc      \
${STR_LOOPS_LL_EXAMPLES_DIR}/%.ll
	@echo "[ 3 ] Optimizing           from: $<"
	@opt ${OPT_PASSES} $< -o $@

######################################################
# [6] create human readable *.ll file from *.bc file #
######################################################
${STR_LOOPS_LL_OPT_EXAMPLES_DIR}/%.ll: \
${STR_LOOPS_BC_OPT_EXAMPLES_DIR}/%.bc
	@echo "[ 4 ] Generating *.ll file from: $<"
	@llvm-dis $< -o $@

########################################
# [7] instrument the optimized bitcode #
########################################
${STR_LOOPS_BC_OPT_INSTRUMENTED}/%:   \
${STR_LOOPS_BC_OPT_EXAMPLES_DIR}/%.bc \
${STR_LOOPS_LL_OPT_EXAMPLES_DIR}/%.ll \
${STR_LOOPS_DIR}/main
	@dir=$$(basename $(notdir $<) .bc);            \
	rm -rf ${STR_LOOPS_BC_OPT_INSTRUMENTED}/$$dir; \
	mkdir  ${STR_LOOPS_BC_OPT_INSTRUMENTED}/$$dir; \
	rm -rf ${STR_LOOPS_LL_OPT_INSTRUMENTED}/$$dir; \
	mkdir  ${STR_LOOPS_LL_OPT_INSTRUMENTED}/$$dir; \
	echo "[ 5 ] Instrumenting bitcode    : $<"
	@${STR_LOOPS_DIR}/main $< $@/$(notdir $<)

###################################################################
# [8] run KLEE of instrumented bitcode(s) & generate *.ll file(s) #
###################################################################
${KLEE_OUTPUT_DIR}/%: ${STR_LOOPS_BC_OPT_INSTRUMENTED}/%
	@rm -rf $@
	@mkdir  $@
	@for f in $$(ls $<);                              \
	do                                                \
		file=$@/$$(basename $$f .bc);                 \
		cp $</$$f /tmp/$$f;                           \
		${KLEE} ${KLEE_FLAGS} /tmp/$$f 2> $$file.txt; \
	done;                                             \
	echo "[ 6 ] Executing KLEE On Bitcode: $$f"
	@for f in $$(ls $<);                              \
	do                                                \
		dir=$(notdir $<);                             \
		bcdir=${STR_LOOPS_BC_OPT_INSTRUMENTED}/$$dir; \
		lldir=${STR_LOOPS_LL_OPT_INSTRUMENTED}/$$dir; \
		bcfile=$$(basename $$f .bc).bc;               \
		llfile=$$(basename $$f .bc).ll;               \
		input=$$bcdir/$$bcfile;                       \
		output=$$lldir/$$llfile;                      \
		llvm-dis $$input -o $$output;                 \
	done;

#######################################################
# [9] echo the validity of the example to a text file #
#######################################################
${STR_LOOPS_STATUS_DIR}/%.status: ${KLEE_OUTPUT_DIR}/%
	@status=0;                                     \
	pattern1=${ASSERTION_FAIL_PATTERN};            \
	pattern2=${KLEE_DONE_PATTERN};                 \
	for f in $$(ls $<);                            \
	do                                             \
		file=$</$$f;                               \
		x=$$(grep -c "$$pattern1" "$$file");       \
		y=$$(grep -c "$$pattern2" "$$file");       \
		z=0;                                       \
		if [ "$$x" -eq "0" ] && [ "$$y" -ne "0" ]; \
		then                                       \
			z=1;                                   \
		fi;                                        \
		status=$$((status || $$z));                \
	done;                                          \
	echo "[ 7 ] Grep-ing for assertions: $$f";     \
	echo $$status > $@

#######################################################################
# [10] accumulate the validity of the examples to a single *.csv file #
#######################################################################
${STATUS_FILE}: ${STR_LOOPS_STATUS__FILES}
	@rm -f $@
	@for f in $$(ls ${STR_LOOPS_STATUS_DIR});             \
	do                                                    \
		file=${STR_LOOPS_STATUS_DIR}/$$f;                 \
	 	status=$$(head -n 1 "$$file");                    \
		printf '%s\n' $$f $$status | paste -sd ',' >> $@; \
	done
	@echo "[ 8 ] Loop over status files and create status.csv"

#########################
# [11] clean target ... #
#########################
clean:
	rm -f  ${STATUS_FILE}
	rm -rf ${KLEE_OUTPUT_DIR}
	mkdir  ${KLEE_OUTPUT_DIR}
	rm -f  ${STR_LOOPS_DIR}/main
	rm -f  ${STR_LOOPS_OBJ_FILES}
	rm -rf ${STR_LOOPS_STATUS_DIR}
	mkdir  ${STR_LOOPS_STATUS_DIR}
	rm -rf ${STR_LOOPS_BC_EXAMPLES_DIR}
	mkdir  ${STR_LOOPS_BC_EXAMPLES_DIR}
	rm -rf ${STR_LOOPS_LL_EXAMPLES_DIR}
	mkdir  ${STR_LOOPS_LL_EXAMPLES_DIR}
	rm -rf ${STR_LOOPS_BC_OPT_EXAMPLES_DIR}
	mkdir  ${STR_LOOPS_BC_OPT_EXAMPLES_DIR}
	rm -rf ${STR_LOOPS_LL_OPT_EXAMPLES_DIR}
	mkdir  ${STR_LOOPS_LL_OPT_EXAMPLES_DIR}
	rm -rf ${STR_LOOPS_BC_OPT_INSTRUMENTED}
	mkdir  ${STR_LOOPS_BC_OPT_INSTRUMENTED}
	rm -rf ${STR_LOOPS_LL_OPT_INSTRUMENTED}
	mkdir  ${STR_LOOPS_LL_OPT_INSTRUMENTED}

######################################
# [12] build_everything_from_scratch #
######################################
build_everything_from_scratch:
	${STR_LOOPS_SCRIPTS_DIR}/build_everything_from_scratch

