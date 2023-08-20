#include "sigHandler.h"
#include "logging.h"

volatile sig_atomic_t shutdown_flag = 0;

void handle_shutdown(int signum) {
    traceError("%s", "Received SHUTDOWN SIGNAL");
    shutdown_flag = 1;
}
