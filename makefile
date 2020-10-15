npshell: npshell.cpp ./utils/utils.cpp ./utils/process.cpp
	g++ -g -o npshell npshell.cpp ./utils/utils.cpp ./utils/process.cpp

commands: ./commands/noop.cpp ./commands/number.cpp ./commands/removetag.cpp ./commands/removetag0.cpp
	g++ -o ./bin/noop ./commands/noop.cpp
	g++ -o ./bin/number ./commands/number.cpp
	g++ -o ./bin/removetag ./commands/removetag.cpp
	g++ -o ./bin/removetag0 ./commands/removetag0.cpp