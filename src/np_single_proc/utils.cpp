#include "utils.h"
#include "cmd.h"
#include "user.h"

#include <unistd.h>
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <fstream>
#include <cstring>
#include <map>
#include <sys/wait.h>



// pipe utils
// stdin -> pipe's read end
void link_pipe_read(int pipe_read) {
   close(STDIN_FILENO);
   dup(pipe_read);
   close(pipe_read);
}

// stdout -> pipe's write end
void link_pipe_write(int pipe_write, bool pipe_err) {
    close(STDOUT_FILENO);
    dup(pipe_write);

    if (pipe_err) {
        close(STDERR_FILENO);
        dup(pipe_write);
    }

    close(pipe_write);
}

void output(
    string filename,
    int prev_pipe_read
) {
    ofstream my_file(filename);
    char buf[BUF_SIZE + 1];
    memset(buf, '\0', BUF_SIZE + 1);

    int num_read;
    while ((num_read = read(prev_pipe_read, buf, BUF_SIZE)) > 0) {
        buf[num_read] = '\0';
        my_file << buf;    
    }
    my_file.close();
}



void decrease_num_pipe(vector<Cmd *> &num_pipe_list) {
    for (size_t i = 0; i < num_pipe_list.size(); i++) {
        num_pipe_list.at(i)->decrease_counter();
    }
}

// close pipe read and erase from list
void erase_num_pipe(vector<Cmd *> &num_pipe_list) {
    for (size_t i = 0; i < num_pipe_list.size(); i++) {
        int counter = num_pipe_list.at(i)->get_counter();
        if (counter == 0) {
            close(num_pipe_list.at(i)->get_pipe_read());
            delete num_pipe_list.at(i);
            num_pipe_list.erase(num_pipe_list.begin() + i);
            i--;
        }
    }
}


int get_pipe_counter(string token) {
    int pipe_counter = 0;

    try {
        pipe_counter = stoi(token.substr(1));
    } catch(const exception &e) {
        pipe_counter = -1;
    }

    return pipe_counter;
}


int pipe_worker(vector<Cmd *> &num_pipe_list) {
    int pipefd[2];
    pipe(pipefd);
    int stdout_tmp = dup(STDOUT_FILENO);
    replace_fd(STDOUT_FILENO, pipefd[1]);
    
    for (size_t i = 0; i < num_pipe_list.size(); i++) {
        if (num_pipe_list.at(i)->get_counter() == 0) {
            char buf[BUF_SIZE + 1];
            int num_read;
            while ((num_read = read(num_pipe_list.at(i)->get_pipe_read(), buf, BUF_SIZE)) > 0) {
                buf[num_read] = '\0';
                cout << buf;
            }
        }
    }
    cout << flush;
    replace_fd(STDOUT_FILENO, stdout_tmp);
    erase_num_pipe(num_pipe_list);
    return pipefd[0];
}

void replace_fd(int ori, int targ) {
    close(ori);
    dup(targ);
    close(targ);
}

// exist at least a pipe that's ready to be read
bool pipe_exist(vector<Cmd *> &num_pipe_list) {
    bool exist = false;
    for (size_t i = 0; i < num_pipe_list.size(); i++)
    {
        if (num_pipe_list.at(i)->get_counter() == 0) {
            exist = true;
            break;
        }
    }

    return exist;
}

void replace_fd(int fd) {
    close(STDIN_FILENO);
    dup(fd);
    close(STDOUT_FILENO);
    dup(fd);
    close(STDERR_FILENO);
    dup(fd);
}


void who(int my_fd) {
    std::map<int, user *>::iterator it;
    std::string head = "<ID>\t<nickname>\t<IP:port>\t<indicate me>\n";
    std::string body;
    for (it = ulist::id_mapper.begin(); it != ulist::id_mapper.end(); it++) {
        user *u = it->second;
        body += to_string(u->get_id());
        body +=  "\t";
        body += u->name;
        body += "\t";
        body += u->get_ip();
        body += "\t";
        if (u->get_sockfd() == my_fd) {
            body += "<- me";
        }
        body += "\n";
    }
    body = head + body;
    write(my_fd, body.c_str(), body.size());
}