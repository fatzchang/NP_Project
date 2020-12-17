#include "firewall.h"
#include <iostream>
#include <regex>

std::ifstream firewall::config;
std::regex ip_reg("(\\d{0,3}|\\*).(\\d{0,3}|\\*).(\\d{0,3}|\\*).(\\d{0,3}|\\*)");
std::regex config_reg("permit (c|b) (\\d{0,3}|\\*).(\\d{0,3}|\\*).(\\d{0,3}|\\*).(\\d{0,3}|\\*)");

void firewall::load() 
{
    config.open("socks.conf");
}

bool firewall::check(std::string ip, MODE mode) 
{
    // std::smatch traffic_match_result;
    std::smatch conf_match_result;
    // regex_match(ip, traffic_match_result, ip_reg);

    load();
    std::string line;
    while(getline(config, line)) {
        regex_match(line, conf_match_result, config_reg);
        //m[1] m[2] m[3] m[4]
        std::cout << conf_match_result[1].str() << std::endl;
        std::cout << conf_match_result[2].str() << std::endl;
        std::cout << conf_match_result[3].str() << std::endl;
        std::cout << conf_match_result[4].str() << std::endl;
        std::cout << conf_match_result[5].str() << std::endl;

        // for (auto &match: m) {
        //     std::cout << match.str() << std::endl;
        // }

        // std::cout << line << std::endl;
    }

    return true;
}