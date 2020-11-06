CPP=g++
CC=gcc
NPSIMPLE_EXEC=np_simple
NPSIMPLE_SRC_DIR=src/np_simple
# NPSIMPLE_INCL_DIR=include/np_simple
NPSIMPLE_SRCS=${wildcard ${NPSIMPLE_SRC_DIR}/*}
NPSIMPLE_OBJS=${patsubst %.c, %.o, ${NPSIMPLE_SRCS}}


NPSINGLEPROC_EXEC=np_single_proc
NPSINGLEPROC_SRC_DIR=src/np_single_proc
# NPSINGLEPROC_INCL_DIR=include/np_single_proc
NPSINGLEPROC_SRCS=${wildcard ${NPSINGLEPROC_SRC_DIR}/*}

INCLUDE_DIR=include


all: np_single_proc np_simple

np_simple:${NPSIMPLE_SRCS}
	${CPP} -o ${NPSIMPLE_EXEC} ${NPSIMPLE_SRCS} -I${INCLUDE_DIR} -std=c++11

np_single_proc: 
	${CPP} -o ${NPSINGLEPROC_EXEC} ${NPSINGLEPROC_SRCS} -I${INCLUDE_DIR} -std=c++11

clean:
	@rm -f ${NPSHELL_EXEC} ${NPSIMPLE_EXEC} ${NPSINGLEPROC_EXEC}