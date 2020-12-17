#include "firewall.h"
#include <iostream>
#include <regex>

std::ifstream firewall::config;
std::regex ip_reg("(\\d{1,3}|\\*).(\\d{1,3}|\\*).(\\d{1,3}|\\*).(\\d{1,3}|\\*)");
std::regex c_reg("permit c (\\d{1,3}|\\*).(\\d{1,3}|\\*).(\\d{1,3}|\\*).(\\d{1,3}|\\*)");
std::regex b_reg("permit b (\\d{1,3}|\\*).(\\d{1,3}|\\*).(\\d{1,3}|\\*).(\\d{1,3}|\\*)");

void firewall::load() 
{
    config.open("socks.conf");
}

void firewall::close() 
{
    config.close();
}

bool firewall::check(std::string ip, MODE mode) 
{
    load();
    // if no config file, directly allow
    if (!config.is_open()) {
        return true;
    }
    
    std::smatch traffic_match_result;
    std::smatch conf_match_result;
    regex_match(ip, traffic_match_result, ip_reg);

    std::string line;
    std::regex config_reg;

    // set the corresponding regex
    if (mode == MODE::CONNECT) {
        config_reg.assign(c_reg);
    } else {
        config_reg.assign(b_reg);
    }

    // feching config file
    bool permit = false;
    while(getline(config, line)) {
        // connect mode only care about c, vice versa
        if (regex_match(line, conf_match_result, config_reg)) {
            std::string conf_match, traffic_match;
            size_t match_counter = 0;

            for (size_t i = 1; i <= 4; i++) {
                conf_match = conf_match_result[i].str();
                traffic_match = traffic_match_result[i].str();

                if ((conf_match == "*") || (conf_match == traffic_match)) {
                    match_counter++;
                }
            }
            permit = (match_counter == 4);
        };


        if (permit) {
            break;
        }
    }

    close();

    return permit;
}