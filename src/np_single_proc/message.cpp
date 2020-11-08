#include "message.h"
#include <string>

std::string welcome_msg() {
    return "****************************************\n** Welcome to the information server. **\n****************************************\n";
}

std::string login_msg(std::string ip, std::string port) {
    std::string msg = "*** User '(no name)' entered from ";
    msg += ip;
    msg += ":";
    msg += port;
    msg += ". ***\n";

    return msg;
}

std::string change_name_msg(std::string name, std::string ip, std::string port) {
    std::string msg;
    msg = "*** User from ";
    msg += ip;
    msg += ":";
    msg += port;
    msg += " is named '";
    msg += name;
    msg += "'. ***\n";

    return msg;
}

std::string logout_msg(std::string name) {
    std::string msg;
    msg = "*** User '";
    msg += name;
    msg += "' left. ***\n";
    
    return msg;
}

std::string yell_msg(std::string name, std::string message) {
    std::string msg;
    msg += "*** ";
    msg += name;
    msg += " yelled ***: ";
    msg += message;
    msg += "\n";

    return msg;
}