CPP=g++
CC=gcc
NPSHELL_EXEC=npshell
NPSHELL_SRC_DIR=src/npshell
NPSHELL_INCL_DIR=include/npshell
NPSHELL_SRCS=${wildcard ${NPSHELL_SRC_DIR}/*}

NPSIMPLE_EXEC=np_simple
NPSIMPLE_SRC_DIR=src/np_simple
NPSIMPLE_INCL_DIR=include/np_simple
NPSIMPLE_SRCS=${wildcard ${NPSIMPLE_SRC_DIR}/*}
NPSIMPLE_OBJS=${patsubst %.c, %.o, ${NPSIMPLE_SRCS}}


NPSINGLEPROC_EXEC=np_single_proc
NPSINGLEPROC_SRC_DIR=src/np_single_proc
NPSINGLEPROC_INCL_DIR=include/np_single_proc
NPSINGLEPROC_SRCS=${wildcard ${NPSINGLEPROC_SRC_DIR}/*}


all: npshell np_simple


npshell: ${NPSHELL_SRCS}
	${CPP} -o ${NPSHELL_EXEC} ${NPSHELL_SRCS} -I${NPSHELL_INCL_DIR} -std=c++11

np_simple:${NPSIMPLE_SRCS}
	${CC} -o ${NPSIMPLE_EXEC} ${NPSIMPLE_SRCS} -I${NPSIMPLE_INCL_DIR}

np_single_proc: 
	${CPP} -o ${NPSINGLEPROC_EXEC} ${NPSINGLEPROC_SRCS} -I${NPSINGLEPROC_INCL_DIR}

clean:
	@rm -f ${NPSHELL_EXEC} ${NPSIMPLE_EXEC} ${NPSINGLEPROC_EXEC}