CPP=g++
EXEC=http_server
SRC_DIR=src
SRCS=${wildcard ${SRC_DIR}/*/*.cpp}
INCL_DIR=include
INCLS=${wildcard ${INCL_DIR}/*.h}
BUILD_DIR=build
OBJS=${patsubst ${SRC_DIR}/%.cpp, ${BUILD_DIR}/%.o, ${SRCS}}

part1: ${OBJS}
	${CPP} ${OBJS} -o ${EXEC} -lboost_system -lpthread
	@echo done

build/%.o: ${SRC_DIR}/%.cpp
	@echo compile $< to ${@D} ...
	@mkdir -p ${@D}
	@${CPP} -c $< -o $@ -I${INCL_DIR} -std=c++11


clean:
	@rm -f ${EXEC}
	@rm -rf ${BUILD_DIR}