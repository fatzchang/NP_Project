#include <map>
#include <iostream>

void render() {
    std::string page;
    page += "<!DOCTYPE html>";
    page += "<html lang=\"en\"><head><meta charset=\"UTF-8\" /><title>NP Project 3 Sample Console</title>";
    page +=     "<head>";
    page +=         "<meta charset=\"UTF-8\" />";
    page +=         "<title>NP Project 3 Sample Console</title>";
    page +=     "</head>";
    page +=     "<body>";
    page +=         "Hello!";
    page +=     "</body>";
    page += "</html>";

    std::cout << page << std::endl;
}