
#ifndef SERVER_H
#define SERVER_H

#include <stdint.h>

#define SERVER_OK 420

class Server_t
{
    // Public
  public:
    Server_t( int32_t port );
    ~Server_t() = default;

    int32_t startServer();
    void    readMessageServer();

    // Private
  private:
    int32_t fd;
    int32_t port;
    char    ipv4[ 16 ];
};

#endif   // SERVER_H