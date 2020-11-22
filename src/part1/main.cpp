#include "server.h"
#include <boost/asio.hpp>
#include <iostream>
#include <sys/wait.h>

int main(int argc, char* argv[])
{
    boost::asio::io_context io_context;
    server s(io_context, atoi(argv[1]));
    
    io_context.run();

    return 0;
}