###############
# DIRECTORIES #
###############
BASEDIR = ${HOME}/GIT

###############
# DIRECTORIES #
###############
STR_LOOPS_DIR             = ${BASEDIR}/for.pldi
PATTERN_MATCH_DIR         = ${BASEDIR}/for.pldi/FOLDER_3_PATTERN_MATCH
EXTRACT_LOOP_FUNCTION_DIR = ${PATTERN_MATCH_DIR}/STEP_1_EXTRACT_LOOP_FUNCTION
CHECK_LOOP_FUNCTION_DIR   = ${PATTERN_MATCH_DIR}/STEP_2_CHECK_LOOP_FUNCTION
STR_LOOPS_PREPROC_DIR     = ${PATTERN_MATCH_DIR}/STEP_0_PREPROC
STR_LOOPS_LOOPS___DIR     = ${PATTERN_MATCH_DIR}/STEP_1_EXTRACT_LOOP_FUNCTION/FOLDER_09_PREPROC_LOOP
STR_LOOPS_STATUS__DIR     = ${PATTERN_MATCH_DIR}/STEP_2_CHECK_LOOP_FUNCTION/FOLDER_09_STATUS_FILES
STR_LOOPS_EXAMPLES_DIR    = ${STR_LOOPS_DIR}/FOLDER_0_EXAMPLES
STR_LOOPS_C_EXAMPLES_DIR  = ${STR_LOOPS_EXAMPLES_DIR}/EXAMPLES_C_SRC_DIR

#########
# FILES #
#########
STATUS_FILE  = ${PATTERN_MATCH_DIR}/status.csv
CHECK_LOOP   = ${CHECK_LOOP_FUNCTION_DIR}/main
EXTRACT_LOOP = ${EXTRACT_LOOP_FUNCTION_DIR}/main

########
# KLEE #
########
KLEE_DIR     = ${BASEDIR}/klee/vanilla.klee
KLEE_SRC_DIR = ${KLEE_DIR}/klee
KLEE_INC_DIR = ${KLEE_SRC_DIR}/include

################
# SOURCE FILES #
################
STR_LOOPS_EXAMPLE_FILES        = $(wildcard  ${STR_LOOPS_C_EXAMPLES_DIR}/*.c)
STR_LOOPS_EXAMPLE_FILES_NOTDIR = $(notdir    ${STR_LOOPS_EXAMPLE_FILES})
STR_LOOPS_PREPROC_FILES_NOTDIR = $(patsubst %.c, %.preproc.c, ${STR_LOOPS_EXAMPLE_FILES_NOTDIR})
STR_LOOPS_LOOP____FILES_NOTDIR = $(patsubst %.c, %.loop.c,    ${STR_LOOPS_EXAMPLE_FILES_NOTDIR})
STR_LOOPS_STATUS__FILES_NOTDIR = $(patsubst %.c, %.status,    ${STR_LOOPS_EXAMPLE_FILES_NOTDIR})
STR_LOOPS_PREPROC_FILES        = $(addprefix ${STR_LOOPS_PREPROC_DIR}/,${STR_LOOPS_PREPROC_FILES_NOTDIR})
STR_LOOPS_LOOP____FILES        = $(addprefix ${STR_LOOPS_LOOPS___DIR}/,${STR_LOOPS_LOOP____FILES_NOTDIR})
STR_LOOPS_STATUS__FILES        = $(addprefix ${STR_LOOPS_STATUS__DIR}/,${STR_LOOPS_STATUS__FILES_NOTDIR})

#####################################
# DONT TOUCH MY GENERATED FILES !!! #
#####################################
.SECONDARY:

######################
# [0] default target #
######################
.DEFAULT_GOAL := ${STATUS_FILE}

####################################################
# [1] preprocess source files to eliminate #define #
####################################################
${STR_LOOPS_PREPROC_DIR}/%.preproc.c: ${STR_LOOPS_C_EXAMPLES_DIR}/%.c
	@echo "[ 0 ] Preprocess C source file: $(notdir $<)"
	@gcc -E -I${KLEE_INC_DIR} $< -o $@

#####################################
# [2] build the extract loop parser #
#####################################
${EXTRACT_LOOP}:
	@echo "[ 1 ] Build extract loop parser"
	@cd ${EXTRACT_LOOP_FUNCTION_DIR} && ${MAKE} -s

############################################
# [3] extract loops from preprocessed file #
############################################
${STR_LOOPS_LOOPS___DIR}/%.loop.c: ${STR_LOOPS_PREPROC_DIR}/%.preproc.c ${EXTRACT_LOOP}
	@echo "[ 2 ] Extract loop from preprocessed file: $(notdir $<)"
	@${EXTRACT_LOOP} $< $@

###################################
# [4] build the check loop parser #
###################################
${CHECK_LOOP}:
	@echo "[ 3 ] build the check loop parser"
	@cd ${CHECK_LOOP_FUNCTION_DIR} && ${MAKE} -s

########################################################
# [5] check the validity of the example to a text file #
########################################################
${STR_LOOPS_STATUS__DIR}/%.status: ${STR_LOOPS_LOOPS___DIR}/%.loop.c ${CHECK_LOOP} 
	@echo "[ 4 ] Check loop : $(notdir $<)"
	@${CHECK_LOOP} $< $@.log > $@ 
	
#######################################################################
# [6] accumulate the validity of the examples to a single *.csv file #
#######################################################################
${STATUS_FILE}: ${STR_LOOPS_STATUS__FILES}
	@rm -f $@
	@for f in $$(ls ${STR_LOOPS_STATUS__DIR}/*.status);      \
	do                                                       \
		file=$$(basename $$f);                               \
	 	status=$$(head -n 1 "$$f");                          \
		printf '%s\n' $$file $$status | paste -sd ',' >> $@; \
	done
	@echo "[ 5 ] Loop over status files and create status.csv"

########################
# [7] clean target ... #
########################
clean:
	rm -f ${STATUS_FILE}
	rm -f ${STR_LOOPS_STATUS__FILES}
	rm -f ${STR_LOOPS_LOOP____FILES}
	rm -f ${STR_LOOPS_PREPROC_FILES}

