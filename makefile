npshell: npshell.cpp ./utils/utils.cpp ./utils/process.cpp ./utils/run.cpp
	g++ -g -Wall -std=c++11 -o npshell npshell.cpp ./utils/utils.cpp ./utils/process.cpp ./utils/run.cpp

commands: ./commands/noop.cpp ./commands/number.cpp ./commands/removetag.cpp ./commands/removetag0.cpp
	g++ -o ./bin/noop ./commands/noop.cpp
	g++ -o ./bin/number ./commands/number.cpp
	g++ -o ./bin/removetag ./commands/removetag.cpp
	g++ -o ./bin/removetag0 ./commands/removetag0.cpp