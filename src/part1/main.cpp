#include "server.h"
#include <boost/asio.hpp>
#include <iostream>
#include <sys/wait.h>

int main()
{
    boost::asio::io_context io_context;
    server s(io_context, 7777);
    
    io_context.run();

    return 0;
}