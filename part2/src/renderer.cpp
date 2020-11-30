#include <map>
#include <iostream>
#include "remote.h"
#include <memory>
#include <sstream>

void insert(int id, std::string text) {
    std::stringstream ss(text);
    std::string tmp;
    std::string script;
    script += "<script>";
    script += "document.getElementById(\"target_" + std::to_string(id) + "\")";
    script += ".insertAdjacentHTML(\"beforeend\", `<span>";
    while(getline(ss, tmp)) {
        script += tmp;
        if (tmp != "% ") {
            script += "<br>";
        }
    }
    script += "</span>`);";
    script += "</script>";

    std::cout << script << std::endl;
}