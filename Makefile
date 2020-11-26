CPP=g++
INCL_DIR=include
INCLS=${wildcard ${INCL_DIR}/*.h}

BUILD_DIR=build;
#part 1
SERVER_EXEC=http_server
SERVER_SRC_DIR=src/part1/http_server
SERVER_SRCS=${wildcard ${SERVER_SRC_DIR}/*.cpp}
SERVER_BUILD_DIR=build/part1/http_server
SERVER_OBJS=${patsubst ${SERVER_SRC_DIR}/%.cpp, ${SERVER_BUILD_DIR}/%.o, ${SERVER_SRCS}}

CGI_EXEC=console.cgi
CGI_SRC_DIR=src/part1/cgi
CGI_SRCS=${wildcard ${CGI_SRC_DIR}/*.cpp}
CGI_BUILD_DIR=build/part1/cgi
CGI_OBJS=${patsubst ${CGI_SRC_DIR}/%.cpp, ${CGI_BUILD_DIR}/%.o, ${CGI_SRCS}}

part1: http_server_unix cgi


http_server_unix: ${SERVER_OBJS}
	@echo build http server
	${CPP} ${SERVER_OBJS} -o ${SERVER_EXEC} -lboost_system -lpthread
	@echo done

cgi: ${CGI_OBJS}
	@echo build console.cgi
	${CPP} ${CGI_OBJS} -o ${CGI_EXEC} -lboost_system -lpthread
	@echo done

${SERVER_BUILD_DIR}/%.o: ${SERVER_SRC_DIR}/%.cpp ${INCLS}
	@echo compile $< to ${@D} ...
	@mkdir -p ${@D}
	@${CPP} -c $< -o $@ -I${INCL_DIR} -std=c++11

${CGI_BUILD_DIR}/%.o: ${CGI_SRC_DIR}/%.cpp ${INCLS}
	@echo compile $< to ${@D} ...
	@mkdir -p ${@D}
	@${CPP} -c $< -o $@ -I${INCL_DIR} -std=c++11


clean:
	@rm -f ${SERVER_EXEC} ${CGI_EXEC}
	@rm -rf ${BUILD_DIR}