#include "firewall.h"
#include "main.h"

int main()
{
    firewall::check("192.168.1.1", MODE::BIND);
    return 0;
}