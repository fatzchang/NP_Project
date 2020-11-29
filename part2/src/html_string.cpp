#include "html_string.h"
#include <string>

std::string panel_html() {
    std::string text("Content-type: text/html\r\n\r\n");
    text += "<!DOCTYPE html><html lang=\"en\"><head><title>NP Project 3 Panel</title>";
    text += "<link rel=\"stylesheet\" href=\"https://cdn.jsdelivr.net/npm/bootstrap@4.5.3/dist/css/bootstrap.min.css\" ";
    text += "integrity=\"sha384-TX8t27EcRE3e/ihU7zmQxVncDAy5uIKz4rEkgIXeMed4M0jlfIDPvg6uqKI2xXr2\" ";
    text += "crossorigin=\"anonymous\" /></header>";
    text += "<body class=\"bg-secondary pt-5\"><form action=\"console.cgi\" method=\"GET\">";
    text += "<table class=\"table mx-auto bg-light\" style=\"width: inherit\">";
    text += "<thead class=\"thead-dark\"><tr><th scope=\"col\">#</th><th scope=\"col\">Host</th><th scope=\"col\">Port</th><th scope=\"col\">Input File</th></tr></thead>";
    text += "<tbody>";

    for (int i = 0; i < 5; i++) {
        text += "<tr><th scope=\"row\" class=\"align-middle\">Session";
        text += std::to_string(i+1) + "</th>";
        text += "<td><div class=\"input-group\">";
        text += "<select name=\"h" + std::to_string(i) + "\" class=\"custom-select\">";
        text += "<option></option>";
        for (int j = 1; j < 13; j++) {
            std::string host("nplinx");
            host += std::to_string(j);
            text += "<option value=\"" + host + ".cs.nctu.edu.tw\">" + host + "</option>";
        }
        text += "</select><div class=\"input-group-append\"><span class=\"input-group-text\">.cs.nctu.edu.tw</span></div></div></td>";
        text += "<td><input name=\"p" + std::to_string(i) + "\" type=\"text\" class=\"form-control\" size=\"5\" /></td>";
        text += "<td><select name=\"f" + std::to_string(i) + "\" class=\"custom-select\"><option></option>";
        for (int j = 1; j < 11; j++) {
            text += "<option value=\"t" + std::to_string(j) + ".txt\">t";
            text += std::to_string(j) + ".txt</option>";
        }
        text += "</select></td></tr>";
    }
    text += "<tr><td colspan=\"3\"></td><td>";
    text += "<button type=\"submit\" class=\"btn btn-info btn-block\">Run</button></td>";
    text += "</tr></tbody>";
    text += "</tbody></table></form></body></html>";

    return text;
}

// std::string console_html() {

// }
