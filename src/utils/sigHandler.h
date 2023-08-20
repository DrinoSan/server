#ifndef SHUTDOWN_H
#define SHUTDOWN_H

#include <csignal>

extern volatile sig_atomic_t shutdown_flag;

void handle_shutdown(int signum);

#endif // SHUTDOWN_H
