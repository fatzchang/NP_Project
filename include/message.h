#ifndef __MESSAGE_H
#define __MESSAGE_H
#include <string>

std::string welcome_msg();
std::string login_msg(std::string ip, std::string port);
std::string change_name_msg(std::string name, std::string ip, std::string port);
std::string logout_msg(std::string name);
std::string yell_msg(std::string name, std::string message);

#endif