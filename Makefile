CPP=g++
EXEC=http_server
SRC_DIR=src
SRCS=${wildcard ${SRC_DIR}/*.cpp}
INCL_DIR=include
INCLS=${wildcard ${INCL_DIR}/*.h}
BUILD_DIR=build
OBJS=${patsubst ${SRC_DIR}/%.cpp, ${BUILD_DIR}/%.o, ${SRCS}}

all: ${OBJS}
	${CPP} ${OBJS} -o ${EXEC} -lboost_system

build/%.o: ${SRC_DIR}/%.cpp
	echo build $@
	@mkdir -p ${@D}
	${CPP} -c $< -o $@ -I${INCL_DIR}


clean:
	rm -f ${EXEC}
	rm -rf ${BUILD_DIR}