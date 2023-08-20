#include "src/server.h"
#include "src/utils/logging.h"
#include "src/utils/sigHandler.h"

int main( void )
{
    std::signal(SIGINT, handle_shutdown);
    std::signal(SIGTERM, handle_shutdown);

    Server_t server{ 8000 };
    server.startServer();

    return 0;
}