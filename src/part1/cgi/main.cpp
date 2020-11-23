#include <stdlib.h>
#include <string>
#include <vector>
#include <iostream>
#include <boost/algorithm/string.hpp>

#include "renderer.h"

int main() {
    std::string q_str(getenv("QUERY_STRING"));
    std::cout << q_str << std::endl;

    std::vector<std::string> splitVec;
    boost::algorithm::split(splitVec, q_str, boost::algorithm::is_any_of("&"));
    for (auto it = splitVec.begin(); it != splitVec.end(); it++) {
        // int index = atoi(it->at(2));

        // std::string s("asdf");
        // std::cout << s.at(2) << std::endl;


    }

    render();
    

    return 0;
}