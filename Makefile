CPP=g++
EXEC=npshell
SRC_DIR=src
INCL_DIR=include
BIN_DIR=bin
MAIN=np
SRCS=${wildcard ${SRC_DIR}/*}



all: ${SRCS} main.cpp
	${CPP} -o ${EXEC} ${SRCS} main.cpp -I${INCL_DIR} -std=c++11

clean:
	@rm -rf ${BIN_DIR}
	@rm -f ${EXEC}