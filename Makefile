CPP=g++
INCL_DIR=include
BUILD_DIR=build
# socks server
SERVER_EXEC=socks_server
SERVER_SRC_DIR=src/server
SERVER_BUILD_DIR=${BUILD_DIR}/server
SERVER_SRCS=${wildcard ${SERVER_SRC_DIR}/*.cpp}
SERVER_OBJS=${patsubst ${SERVER_SRC_DIR}/%.cpp, ${SERVER_BUILD_DIR}/%.o, ${SERVER_SRCS}}

# cgi
CGI_EXEC=hw4.cgi
CGI_SRC_DIR=src/cgi
CGI_BUILD_DIR=${BUILD_DIR}/cgi
CGI_SRCS=${wildcard ${CGI_SRC_DIR}/*.cpp}
CGI_OBJS=${patsubst ${CGI_SRC_DIR}/%.cpp, ${CGI_BUILD_DIR}/%.o, ${CGI_SRCS}}


all: ${SERVER_EXEC} ${CGI_EXEC}

${SERVER_EXEC}: ${SERVER_OBJS}	
	${CPP} -o ${SERVER_EXEC} ${SERVER_OBJS} -lpthread

${SERVER_BUILD_DIR}/%.o: ${SERVER_SRC_DIR}/%.cpp ${INCL_DIR}/%.h
	@mkdir -p ${@D}
	${CPP} -c $< -o $@ -I${INCL_DIR} -std=c++11






${CGI_EXEC}: ${CGI_OBJS}
	${CPP} -o ${CGI_EXEC} ${CGI_OBJS} -lpthread

${CGI_BUILD_DIR}/%.o: ${CGI_SRC_DIR}/%.cpp ${INCL_DIR}/%.h
	@mkdir -p ${@D}
	${CPP} -c $< -o $@ -I${INCL_DIR} -std=c++11


clean:
	@rm -rf ${BUILD_DIR}
	@rm -f ${SERVER_EXEC} ${CGI_EXEC}
