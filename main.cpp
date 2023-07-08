#include "src/server.h"
#include "src/utils/logging.h"

int main( void )
{
    Server_t server{ 8000 };
    server.startServer();
    server.readMessageServer();

    return 0;
}