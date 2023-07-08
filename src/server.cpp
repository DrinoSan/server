#include "server.h"
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

// Project Headers
#include "utils/logging.h"

Server_t::Server_t( int32_t port ) : port{ port } {}

//-----------------------------------------------------------------------------
void Server_t::readMessageServer()
{
    // Waiting for client
    struct sockaddr_storage client_addr = {};
    socklen_t               socklen     = sizeof( client_addr );

    while ( true )
    {
        traceInfo( "Waiting..." );
        int connfd = accept( fd, ( struct sockaddr* ) &client_addr, &socklen );

        if ( connfd < 0 )
        {
            traceError( "Failed on accept, error value: %d", connfd );
            return;
        }

        char rbuf[ 100 ];
        read( connfd, rbuf, sizeof( rbuf ) );
        traceInfo( "Got message from Client: %s", rbuf );

        const char wbuf[ 19 ] = "Hello from Server!";
        write( connfd, wbuf, sizeof( wbuf ) );
        close( connfd );
    }
}

//-----------------------------------------------------------------------------
int32_t Server_t::startServer()
{
    fd = socket( PF_INET, SOCK_STREAM, 0 );
    if ( fd == -1 )
    {
        traceError( "Failed on creating socket" );
        return fd;
    }

    int opt = 1;
    if ( setsockopt( fd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof( opt ) ) )
    {
        traceError( "Failed setsockopt" );
    }

    struct sockaddr_in addr = {};
    addr.sin_family         = AF_INET;
    addr.sin_port           = ntohs( port );
    addr.sin_addr.s_addr    = ntohl( 0 );

    int rv;
    rv = bind( fd, ( const struct sockaddr* ) &addr, sizeof( addr ) );
    if ( rv < 0 )
    {
        traceError( "Failed on bind" );
        return rv;
    }

    rv = listen( fd, 5 );   // 5 is the backlog...
    if ( rv == -1 )
    {
        traceError( "Error on listening" );
        return rv;
    }

    return SERVER_OK;
}