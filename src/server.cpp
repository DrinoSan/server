#include "server.h"
#include <arpa/inet.h>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/_types/_socklen_t.h>
#include <sys/errno.h>
#include <sys/event.h>
#include <sys/socket.h>
#include <unistd.h>

// Project Headers
#include "httpRequest.h"
#include "parser.h"
#include "utils/logging.h"

//----------------------------------------------------------------------------
Server_t::Server_t( int32_t port ) : port{ port }
{
    traceInfo( "%s", "Setting up kqueue fd's" );
    for ( uint8_t idx = 0; idx < NUM_WORKERS; ++idx )
    {
        working_kqueue_fd[ idx ] = kqueue();
        traceInfo( "worker %d got %d kqueue descripter", idx,
                   working_kqueue_fd[ idx ] );
    }
}

//----------------------------------------------------------------------------
void* getInAddr( struct sockaddr* sa )
{
    if ( sa->sa_family == AF_INET )
    {
        return &( ( ( struct sockaddr_in* ) sa )->sin_addr );
    }

    return &( ( ( struct sockaddr_in6* ) sa )->sin6_addr );
}

//-----------------------------------------------------------------------------
void Server_t::listenAndAccept()
{
    int8_t worker_idx{ 0 };

    while ( true )
    {
        traceInfo( "%s", "Waiting for connection..." );

        struct sockaddr_storage their_addr;   // connectors address information
        socklen_t               sin_size;
        auto                    otherSockConnection = new otherSockInfo_t;
        otherSockConnection->sockfd =
            accept( sockfd, ( struct sockaddr* ) &their_addr, &sin_size );

        if ( otherSockConnection->sockfd == -1 )
        {
            traceError( "Accept socket failed with: %d", otherSockConnection );
            delete otherSockConnection;
            continue;
        }

        // Obtaining address family
        getpeername( otherSockConnection->sockfd,
                     ( struct sockaddr* ) &their_addr, &sin_size );

        // This should be dynamic and not hardcoded to ipv4
        char s[ INET_ADDRSTRLEN ];
        // Cleaning trash from memory
        memset( s, '\0', INET_ADDRSTRLEN );

        inet_ntop( their_addr.ss_family,
                   getInAddr( ( struct sockaddr* ) &their_addr ), s,
                   sizeof s );

        traceInfo( "Got connection from: %s with address family: %d", s,
                   their_addr.ss_family );

        EV_SET( working_chevents[ worker_idx ], otherSockConnection->sockfd,
                EVFILT_READ, EV_ADD, 0, 0, otherSockConnection );

        if ( kevent( working_kqueue_fd[ worker_idx ],
                     working_chevents[ worker_idx ], 1, nullptr, 0,
                     nullptr ) < 0 )
        {
            traceError( "Kevent error: %d", errno );
            if ( errno != 0 )
            {
                exit( EXIT_FAILURE );
            }

            continue;
        }

        ++worker_idx;
        if ( worker_idx == NUM_WORKERS )
        {
            worker_idx = 0;
        }
    }
}

//-----------------------------------------------------------------------------
void Server_t::processWorkerEvents( int8_t worker_idx )
{
    int64_t new_events;   //, socket_connection_fd, client_len;

    // File descriptor for kqueue
    auto worker_kfd = working_kqueue_fd[ worker_idx ];

    while ( true )
    {
        new_events =
            kevent( worker_kfd, nullptr, 0, working_events[ worker_idx ],
                    NUM_EVENTS, nullptr );

        if ( new_events == -1 )
        {
            traceError( "Kevent error: %d", errno );
            exit( EXIT_FAILURE );
        }

        for ( uint32_t i = 0; i < new_events; ++i )
        {
            int32_t event_fd = working_events[ worker_idx ][ i ].ident;

            Server_t::otherSockInfo_t* sockInfo =
                reinterpret_cast<Server_t::otherSockInfo_t*>(
                    working_events[ worker_idx ][ i ].udata );

            // When the client disconnects an EOF is sent. By
            // closing the file descriptor the event is
            // automatically removed from the kqueue
            if ( working_events[ worker_idx ][ i ].flags & EV_EOF )
            {
                traceInfo( "%s", "Client has disconnected" );

                while ( close( sockInfo->sockfd ) == -1 )
                {
                    traceError( "ERRNO: %d", errno );
                }
                delete sockInfo;
            }
            // If the new event's file descriptor is the same as the
            // listening socket's file descriptor, we are sure that
            // a new client wants to connect to our socket.
            else if ( event_fd == sockfd )
            {
                traceError( "%s", "Check me, if you can read me" );
                continue;
            }
            else if ( working_events[ worker_idx ][ i ].filter & EVFILT_READ )
            {
                traceInfo( "%s", "Handling read and write" );
                // Read bytes from socket
                HttpRequest_t httpRequest = handleRead( sockInfo );
                handleWrite( httpRequest, sockInfo->sockfd );
            }
        }
    }
}

//-----------------------------------------------------------------------------
HttpRequest_t Server_t::handleRead( Server_t::otherSockInfo_t* sockInfo )
{
    HttpRequest_t httpRequest;
    memset( httpRequest.buffer, '\0', BUFFER_SIZE );

    int32_t bytesRead;
    while ( ( bytesRead = recv( sockInfo->sockfd, httpRequest.buffer,
                                BUFFER_SIZE, 0 ) ) )
    {
        if ( bytesRead <= 0 )
        {
            // We have a error or client closed connection
            traceError(  "[handleRead] bytesRead: %d", bytesRead );
            break;
        }

        if ( strstr( httpRequest.buffer, "\r\n\r\n" ) )
        {
            // We found a sane end
            break;
        }
    }

    Parser_t parser;
    parser.mapRequestHeaders( httpRequest );

    traceInfo( "%s", "Returning from handleRead" );

    return httpRequest;
}

//-----------------------------------------------------------------------------
size_t sendAll( HttpRequest_t& httpRequest, int32_t connfd )
{
    size_t dataSent  = 0;
    size_t chunkSize = 0;

    std::string echo{ httpRequest.buffer };

    while ( dataSent < echo.size() )
    {
        chunkSize = echo.size() - dataSent;
        if ( chunkSize > CHUNK_SIZE )
        {
            chunkSize = CHUNK_SIZE;
        }

        auto bytesSent = send( connfd, echo.data() + dataSent, chunkSize, 0 );
        dataSent += bytesSent;
    }

    traceInfo( "SENT %d bytes", dataSent );
    return dataSent;
}

//-----------------------------------------------------------------------------
void Server_t::handleWrite( HttpRequest_t& httpRequest, int32_t connfd )
{
    traceInfo( "%s", "HandleWrite" );
    sendAll( httpRequest, connfd );
}

//-----------------------------------------------------------------------------
int32_t Server_t::startServer()
{
    sockfd = socket( PF_INET, SOCK_STREAM, 0 );
    if ( sockfd == -1 )
    {
        traceError( "%s", "Failed on creating socket" );
        return sockfd;
    }

    int opt = 1;
    if ( setsockopt( sockfd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof( opt ) ) )
    {
        traceError( "%s", "Failed setsockopt" );
    }

    struct sockaddr_in addr = {};
    addr.sin_family         = AF_INET;
    addr.sin_port           = ntohs( port );
    addr.sin_addr.s_addr    = ntohl( 0 );

    int rv;
    rv = bind( sockfd, ( const struct sockaddr* ) &addr, sizeof( addr ) );
    if ( rv < 0 )
    {
        traceError( "%s", "Failed on bind" );
        return rv;
    }

    rv = listen( sockfd, BACK_LOG );
    if ( rv == -1 )
    {
        traceError( "%s", "Error on listening" );
        return rv;
    }

    // start listnener thread here for incoming connections
    listenerThread = std::thread( &Server_t::listenAndAccept, this );

    // Setting up worker threads
    for ( int i = 0; i < NUM_WORKERS; ++i )
    {
        workerThread[ i ] =
            std::thread( &Server_t::processWorkerEvents, this, i );
    }

    listenerThread.join();
    for ( int i = 0; i < NUM_WORKERS; ++i )
    {
        workerThread[ i ].join();
    }

    return SERVER_OK;
}