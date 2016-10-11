/*
 * Device func declaration
 */
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <syslog.h>
#include <libpq-fe.h>

#include "settings.h"
#include "sockutils.h"
#include "fifo.h"

void proto(const int* client_fd, PGconn *conn);
void *threadFunc(void *arg);

int db_login(PGconn **conn);
int execsql(PGconn *conn, char *sql, char *report);
PGresult *getexecsql(PGconn *conn, char * sql);
void clearres(PGconn *conn, PGresult *res);
