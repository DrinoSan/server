
#ifndef SERVER_H
#define SERVER_H

#include <stdint.h>
#include <sys/event.h>
#include <sys/time.h>
#include <sys/types.h>
#include <thread>
#include <csignal>

// Project Headers
#include "httpRequest.h"

#define SERVER_OK 420

constexpr int     BACK_LOG    = 10000;
constexpr int32_t NUM_WORKERS = 5;
constexpr int32_t NUM_EVENTS  = 100;
constexpr int32_t CHUNK_SIZE  = 1000;

class Server_t
{
    // Public
  public:
    Server_t( int32_t port );
    ~Server_t() = default;

    int32_t startServer();

    // Private
  private:
    int32_t sockfd;
    int32_t port;

    // Kqueue stuff
    int           working_kqueue_fd[ NUM_WORKERS ];
    struct kevent working_events[ NUM_WORKERS ][ NUM_EVENTS ];
    struct kevent working_chevents[ NUM_WORKERS ][ NUM_EVENTS ];

    // Threads
    std::thread listenerThread;
    std::thread workerThread[ NUM_WORKERS ];

    struct otherSockInfo_t
    {
        int32_t sockfd;
    };

    void          listenAndAccept();
    void          processWorkerEvents( int8_t worker_idx );
    HttpRequest_t handleRead( otherSockInfo_t* sockInfo );
    void          handleWrite( HttpRequest_t& httpRequest, int32_t connfd );
};

#endif   // SERVER_H