#include <map>
#include <iostream>
#include "remote.h"

void render(std::map<int, remote*> &remote_list) {
    std::string page;
    page += "<!DOCTYPE html>";
    page += "<html lang=\"en\"><head><meta charset=\"UTF-8\" /><title>NP Project 3 Sample Console</title>";
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
    page +=                     it->second->host() + ":" + std::to_string(it->second->port());
    page +=                 "</div>";
        
    }

    page +=             "</div>";
    page +=         "</div>";
    

    page +=     "</body>";
    page += "</html>";
    std::cout << page << std::endl;
}


void insert(int id, std::string text) {
    std::string script;
    script += "<script>";
    script += "document.getElementById(\"target_" + std::to_string(id) + "\")";
    script += ".insertAdjacentHTML(\"beforeend\", ";
    script += "\"<div>" + text + "</div>\"";
    script += "</script>";
}