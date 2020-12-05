#include "server.h"
#include <boost/asio.hpp>

int main(int argc, char *argv[])
{
    boost::asio::io_context ioc;
    server s(ioc, 7777);
    ioc.run();
    return 0;
}