CPP=g++
EXEC=sockserver
SRC_DIR=src
BUILD_DIR=build
INCL_DIR=include

SRCS=${wildcard ${SRC_DIR}/*.cpp}
OBJS=${patsubst ${SRC_DIR}/%.cpp, ${BUILD_DIR}/%.o, ${SRCS}}

all: ${OBJS}
	
	${CPP} -o ${EXEC} ${OBJS} -lpthread

${BUILD_DIR}/%.o: ${SRC_DIR}/%.cpp ${INCL_DIR}/%.h
	@mkdir -p ${@D}
	${CPP} -c $< -o $@ -I${INCL_DIR} -std=c++11

clean:
	@rm -rf ${BUILD_DIR}
	@rm -rf ${EXEC}