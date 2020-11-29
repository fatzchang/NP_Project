CPP=g++
PART1_INCLS=${wildcard ${PART1_INCL_DIR}/*.h}

PART1_BUILD_DIR=part1/build
PART2_BUILD_DIR=part2/build
BUILD_DIRS=${PART1_BUILD_DIR} ${PART2_BUILD_DIR}
#part 1
PART1_INCL_DIR=part1/include
SERVER_EXEC=http_server
SERVER_SRC_DIR=part1/src/http_server
SERVER_SRCS=${wildcard ${SERVER_SRC_DIR}/*.cpp}
SERVER_BUILD_DIR=${PART1_BUILD_DIR}/http_server
SERVER_OBJS=${patsubst ${SERVER_SRC_DIR}/%.cpp, ${SERVER_BUILD_DIR}/%.o, ${SERVER_SRCS}}

CGI_EXEC=console.cgi
CGI_SRC_DIR=part1/src/cgi
CGI_SRCS=${wildcard ${CGI_SRC_DIR}/*.cpp}
CGI_BUILD_DIR=${PART1_BUILD_DIR}/cgi
CGI_OBJS=${patsubst ${CGI_SRC_DIR}/%.cpp, ${CGI_BUILD_DIR}/%.o, ${CGI_SRCS}}

#part 2
PART2_INCL_DIR=part2/include
PART2_INCLS=${wildcard ${PART2_INCL_DIR}/*.h}
CGI_SERVER_EXEC=cgi_server.exe
CGI_SERVER_SRC_DIR=part2/src
CGI_SERVER_SRCS=${wildcard ${CGI_SERVER_SRC_DIR}/*.cpp}
CGI_SERVER_BUILD_DIR=${PART2_BUILD_DIR}
CGI_SERVER_OBJS=${patsubst ${CGI_SERVER_SRC_DIR}/%.cpp, ${CGI_SERVER_BUILD_DIR}/%.o, ${CGI_SERVER_SRCS}}


part1: http_server_unix cgi
part2: cgi_server

http_server_unix: ${SERVER_OBJS}
	@echo build http server
	${CPP} ${SERVER_OBJS} -o ${SERVER_EXEC} -lboost_system -lpthread
	@echo done

cgi: ${CGI_OBJS}
	@echo build console.cgi
	${CPP} ${CGI_OBJS} -o ${CGI_EXEC} -lboost_system -lpthread
	@echo done

cgi_server: ${CGI_SERVER_OBJS}
	@echo build cgi server
	${CPP} ${CGI_SERVER_OBJS} -o ${CGI_SERVER_EXEC} -lws2_32 -lwsock32
	@echo done

${SERVER_BUILD_DIR}/%.o: ${SERVER_SRC_DIR}/%.cpp ${PART1_INCLS}
	@echo compile $< to ${@D} ...
	@mkdir -p ${@D}
	@${CPP} -c $< -o $@ -I${PART1_INCL_DIR} -std=c++11

${CGI_BUILD_DIR}/%.o: ${CGI_SRC_DIR}/%.cpp ${PART1_INCLS}
	@echo compile $< to ${@D} ...
	@mkdir -p ${@D}
	@${CPP} -c $< -o $@ -I${PART1_INCL_DIR} -std=c++11

${CGI_SERVER_BUILD_DIR}/%.o: ${CGI_SERVER_SRC_DIR}/%.cpp ${PART2_INCLS}
	@echo compile $< to ${@D} ...
	@mkdir -p ${@D}
	@${CPP} -c $< -o $@ -I${PART2_INCL_DIR} -std=c++11



clean:
	@rm -f ${SERVER_EXEC} ${CGI_EXEC} ${CGI_SERVER_EXEC}
	@rm -rf ${BUILD_DIRS}
