#include <map>
#include <iostream>
#include "remote.h"
#include <memory>
#include <sstream>

void render(std::map<int, std::shared_ptr<remote>> &remote_list) {
    std::string page;
    page += "Content-type: text/html\r\n\r\n";
    page += "<!DOCTYPE html>";
    page += "<html lang=\"en\">";
    page +=     "<head>";
    page +=         "<meta charset=\"UTF-8\" />";
    page +=         "<title>NP Project 3 Sample Console</title>";
    page +=         "<link rel=\"stylesheet\" href=\"https://cdn.jsdelivr.net/npm/bootstrap@4.5.3/dist/css/bootstrap.min.css\" integrity=\"sha384-TX8t27EcRE3e/ihU7zmQxVncDAy5uIKz4rEkgIXeMed4M0jlfIDPvg6uqKI2xXr2\" crossorigin=\"anonymous\">";
    page +=     "</head>";
    page +=     "<body>";
    page +=         "<div class=\"container-fluid\">";
    page +=             "<div class=\"row\">";
    
    for (auto it = remote_list.begin(); it != remote_list.end(); it++) {

    page +=                 "<div class=\"col\" id=\"target_" + std::to_string(it->second->id()) + "\">";
    page +=                     "<div>" + it->second->host() + ":" + std::to_string(it->second->port()) + "</div>";
    page +=                 "</div>";
        
    }

    page +=             "</div>";
    page +=         "</div>";
    

    page +=     "</body>";
    page += "</html>";
    std::cout << page << std::endl;
}


void insert(int id, std::string text) {
    std::stringstream ss(text);
    std::string tmp;
    std::string script;
    script += "<script>";
    script += "document.getElementById(\"target_" + std::to_string(id) + "\")";
    script += ".insertAdjacentHTML(\"beforeend\", `<span>";
    while(getline(ss, tmp)) {
        if (tmp.size() != 0) {
            // std::cerr << "tmp is: " << tmp << ", is empty: " << (tmp == "\n" || tmp == "\r\n") << std::endl;
            // std::cerr << "tmp length is: " << tmp.size() << std::endl;
            script += tmp;
            if (tmp != "% ") {
                script += "<br>";

            }
        }
    }
    script += "</span>`);";
    script += "</script>";

    std::cout << script << std::endl;
}