#include <vector>
#include <unistd.h>
#include <string>
#define FIRST_COMMAND (1)
#define LAST_COMMAND (2)
#define MEDIUM_COMMAND (3)
#define ONLY_COMMAND (4)

void link_pipe_read(int[2]);
void link_pipe_write(int[2]);

pid_t run_command(std::vector<char*>, int, int[2][2], int, bool);

pid_t output(std::vector<char*>, std::string, int[2][2], int);