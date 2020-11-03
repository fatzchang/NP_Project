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


all: npshell np_simple


npshell: ${NPSHELL_SRCS}
	${CPP} -o ${NPSHELL_EXEC} ${NPSHELL_SRCS} -I${NPSHELL_INCL_DIR} -std=c++11

np_simple: 
	${CC} -o ${NPSIMPLE_EXEC} ${NPSIMPLE_SRCS} -I${NPSIMPLE_INCL_DIR}

clean:
	@rm -rf ${BIN_DIR}
	@rm -f ${NPSHELL_EXEC}