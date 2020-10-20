npshell: npshell.cpp ./utils/utils.cpp ./utils/process.cpp ./utils/run.cpp
	g++ -std=c++11 -o npshell npshell.cpp ./utils/utils.cpp ./utils/process.cpp ./utils/run.cpp

debug: npshell.cpp ./utils/utils.cpp ./utils/process.cpp ./utils/run.cpp
	g++ -g -Wall -std=c++11 -o npshell npshell.cpp ./utils/utils.cpp ./utils/process.cpp ./utils/run.cpp