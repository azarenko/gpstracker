/*
 * Device func declaration
 */
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <syslog.h>

#include "settings.h"
#include "sockutils.h"
#include "fifo.h"

void proto(const int* client_fd);
void *threadFunc(void *arg);

