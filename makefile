###############
# DIRECTORIES #
###############
BASEDIR = ${HOME}/GIT

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
STR_LOOPS_EXAMPLES_DIR        = ${STR_LOOPS_DIR}/FOLDER_0_EXAMPLES
STR_LOOPS_C_EXAMPLES_DIR      = ${STR_LOOPS_EXAMPLES_DIR}/EXAMPLES_C_SRC_DIR
STR_LOOPS_BC_EXAMPLES_DIR     = ${STR_LOOPS_EXAMPLES_DIR}/EXAMPLES_BC_DIR
STR_LOOPS_BC_OPT_EXAMPLES_DIR = ${STR_LOOPS_EXAMPLES_DIR}/EXAMPLES_BC_OPT_DIR
STR_LOOPS_BC_OPT_INSTRUMENTED = ${STR_LOOPS_EXAMPLES_DIR}/EXAMPLES_BC_OPT_INSTRUMENTED_DIR
STR_LOOPS_STATUS_DIR          = ${STR_LOOPS_EXAMPLES_DIR}/EXAMPLES_STATUS_DIR

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
KLEE_BIN_DIR = ${BASEDIR}/klee/vanilla.klee/build/bin
KLEE = ${KLEE_BIN_DIR}/klee
ASSERTION_FAIL_PATTERN="ASSERTION FAIL"
KLEE_DONE_PATTERN="KLEE: done"

################
# SOURCE FILES #
################
STR_LOOPS_SRC_FILES        = $(wildcard  ${STR_LOOPS_SRC_DIR}/*.cpp)
STR_LOOPS_SRC_FILES_NOTDIR = $(notdir    ${STR_LOOPS_SRC_FILES})
STR_LOOPS_OBJ_FILES_NOTDIR = $(patsubst %.cpp, %.cpp.o, ${STR_LOOPS_SRC_FILES_NOTDIR})
STR_LOOPS_OBJ_FILES        = $(addprefix ${STR_LOOPS_OBJ_DIR}/,${STR_LOOPS_OBJ_FILES_NOTDIR})
STR_LOOPS_HEADER_FILES     = $(wildcard  ${STR_LOOPS_INC_DIR}/*.h)
STR_LOOPS_STATUS_FILES     = $(wildcard  ${STR_LOOPS_STATUS_DIR}/*.txt)

#################
# BITCODE FILES #
#################
STR_LOOPS_BC_OPT_INSTRUMENTED_FILES = $(wildcard ${STR_LOOPS_BC_OPT_INSTRUMENTED}/*.bc)

###########################
# RUN ANALYSIS ON BITCODE #
###########################
all: ${STATUS_FILE}

################################################
# [1] compile source file(s) to object file(s) #
################################################
${STR_LOOPS_OBJ_DIR}/%.cpp.o: ${STR_LOOPS_SRC_DIR}/%.cpp ${STR_LOOPS_HEADER_FILES}
	g++ -g ${DFLAGS} ${IFLAGS} -o $@ -c $<

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
${STR_LOOPS_BC_OPT_INSTRUMENTED}/%:   \
${STR_LOOPS_BC_OPT_EXAMPLES_DIR}/%.bc \
${STR_LOOPS_BC_EXAMPLES_DIR}/%.ll     \
${STR_LOOPS_DIR}/main
	mkdir $@ && ${STR_LOOPS_DIR}/main $< $@/$<

#################################
# [7] human readable *.ll files #
#################################
#${STR_LOOPS_BC_OPT_INSTRUMENTED}/%/%.ll: ${STR_LOOPS_BC_OPT_INSTRUMENTED}/%/%.bc
#	llvm-dis -o $@ $<

########################################
# [8] run KLEE of instrumented bitcode #
########################################
${KLEE_OUTPUT_DIR}/%: ${STR_LOOPS_BC_OPT_INSTRUMENTED}/% ${STR_LOOPS_BC_OPT_INSTRUMENTED}/%.ll
	${KLEE} --posix-runtime $< > $@

#############################################
# [9] grep KLEE output for assertion errors #
#############################################
${KLEE_OUTPUT_DIR}/KLEE_STATUS_%.txt: ${KLEE_OUTPUT_DIR}/KLEE_OUTPUT_%.txt
	AssertionHits0=$(grep -c "$ASSERTION_FAIL_PATTERN" ${KLEE_OUTPUT_FILE_0})
	AssertionHits1=$(grep -c "$ASSERTION_FAIL_PATTERN" ${KLEE_OUTPUT_FILE_1})
	AssertionHits2=$(grep -c "$ASSERTION_FAIL_PATTERN" ${KLEE_OUTPUT_FILE_2})
	AssertionHits3=$(grep -c "$ASSERTION_FAIL_PATTERN" ${KLEE_OUTPUT_FILE_3})
	KLEE_Finished0=$(grep -c "$KLEE_DONE_PATTERN"      ${KLEE_OUTPUT_FILE_0})
	KLEE_Finished1=$(grep -c "$KLEE_DONE_PATTERN"      ${KLEE_OUTPUT_FILE_1})
	KLEE_Finished2=$(grep -c "$KLEE_DONE_PATTERN"      ${KLEE_OUTPUT_FILE_2})
	KLEE_Finished3=$(grep -c "$KLEE_DONE_PATTERN"      ${KLEE_OUTPUT_FILE_3})
	status0=[[ "$AssertionHits0" -eq "0" ]] && [[ "$KLEE_Finished0" -ne "0" ]]
	status1=[[ "$AssertionHits1" -eq "0" ]] && [[ "$KLEE_Finished1" -ne "0" ]]
	status2=[[ "$AssertionHits2" -eq "0" ]] && [[ "$KLEE_Finished2" -ne "0" ]]
	status3=[[ "$AssertionHits3" -eq "0" ]] && [[ "$KLEE_Finished3" -ne "0" ]]
	status=${status0}||${status1}||${status2}||${status3}

	echo "$filename: Valid Loop"

########################################################
# [10] echo the validity of the example to a text file #
########################################################
${STR_LOOPS_STATUS_DIR}/%.txt: ${KLEE_OUTPUT_DIR}/%
	status = 0                                   \
	pattern1 = $ASSERTION_FAIL_PATTERN;          \
	pattern2 = $KLEE_DONE_PATTERN;               \
	@for filename in $(shell ls $<);             \
	do                                           \
		x = $(grep -c "$pattern1" $${filename}); \
		y = $(grep -c "$pattern2" $${filename}); \
		x_eq_0 = [[ "$x" -eq "0" ]];             \
		y_ne_0 = [[ "$y" -ne "0" ]];             \
		z = x_eq_0 && y_ne_0;                    \
		status = $((status||$z));                \
	done                                         \
	echo ${status} > $@

#######################################################################
# [11] accumulate the validity of the examples to a single *.csv file #
#######################################################################
${STATUS_FILE}: ${STR_LOOPS_STATUS_FILES}
	@for f in $$(ls ${STR_LOOPS_STATUS_DIR});             \
	do                                                    \
		file=${STR_LOOPS_STATUS_DIR}/$$f;                 \
	 	status=$$(head -n 1 "$$file");                    \
		printf '%s\n' $$f $$status | paste -sd ',' >> $@; \
	done

#########################
# [11] clean target ... #
#########################
clean:
	rm -f ${STR_LOOPS_DIR}/main
	rm -f ${STR_LOOPS_OBJ_FILES}
	rm -f ${STR_LOOPS_BC_EXAMPLES_DIR}/*.bc
	rm -f ${STR_LOOPS_BC_EXAMPLES_DIR}/*.ll
	rm -f ${STR_LOOPS_BC_OPT_EXAMPLES_DIR}/*.bc
	rm -f ${STR_LOOPS_BC_OPT_EXAMPLES_DIR}/*.ll
	rm -f ${STR_LOOPS_BC_OPT_INSTRUMENTED}/*.bc
	rm -f ${STR_LOOPS_BC_OPT_INSTRUMENTED}/*.ll
	
