#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <err.h>
#include <event.h>
#include <signal.h>
#include <libconfig.h>
#include <syslog.h>
#include <pthread.h>

#include "cmdparam.h"
#include "settings.h"
#include "sockutils.h"
#include "devices.h"
#include "fifo.h"

static struct event_base *evbase;

/**
 * Set a socket to non-blocking mode.
 */
static int setnonblock(int fd) {
	int flags;

	flags = fcntl(fd, F_GETFL);
	if (flags < 0) return flags;
	flags |= O_NONBLOCK;
	if (fcntl(fd, F_SETFL, flags) < 0) return -1;
	return 0;
}

/*
 * Terminate server
 */
static void terminate(void)
{
    if (event_base_loopexit(evbase, NULL)) {
		syslog(LOG_ERR, "Error shutting down server");
	}
}

/*
 * Heandling signal
 */
static void sighandler(int signal) {
    switch(signal)
    {
        case SIGTERM: 
            syslog(LOG_INFO, "Received signal terminate.");
            terminate();
            break;
        case SIGHUP:
            syslog(LOG_INFO, "Received signal hangup.");
            if(readSettings(configFilePath) == EXIT_FAILURE)
            {
                syslog(LOG_ERR, "Configuration file error.");                
            }
            break;
    }	
}

/*
 * Accept function. prepare data
 */
void on_accept(int fd, short ev, void *arg) {
	int client_fd;
	struct sockaddr_in client_addr;
	socklen_t client_len = sizeof(client_addr);	

	client_fd = accept(fd, (struct sockaddr *)&client_addr, &client_len);
	if (client_fd < 0) {
		syslog(LOG_ERR, "accept failed");
		return;
	}

	/* Set the client socket to non-blocking mode. */
	if (setnonblock(client_fd) < 0) {
		syslog(LOG_ERR, "failed to set client socket to non-blocking");
		close(client_fd);
		return;
	}	
	
	fifo_put(client_fd);
}


/*
 * Main function
 */
int main(int argc, char *argv[]) 
{
    /*
     * init syslog
     */
    openlog(argv[0], 0, LOG_USER);
    syslog(LOG_INFO, "Starting.");
    
    /*
     * parse command line params
     */
    if(getCmdParam(argc, argv, "-f", &configFilePath) == EXIT_FAILURE)
    {
        syslog(LOG_ERR, "Invalid input params. Use -f path_to_config.");
        goto exit;
    }
    
    /*
     * read settings
     */
    if(readSettings(configFilePath) == EXIT_FAILURE)
    {
        syslog(LOG_ERR, "Configuration file error.");
        goto exit;
    }
    
    /* Set signal handlers */
	sigset_t sigset;
	sigemptyset(&sigset);
	struct sigaction siginfo = {
		.sa_handler = sighandler,
		.sa_mask = sigset,
		.sa_flags = SA_RESTART,
	};
	sigaction(SIGHUP, &siginfo, NULL);
	sigaction(SIGTERM, &siginfo, NULL);
    
    /* Create our listening socket. */
	int listenfd = socket(AF_INET, SOCK_STREAM, 0);
	if (listenfd < 0) {
		syslog(LOG_ERR, "socket listen failed");
        goto exit;
	}
	
	struct sockaddr_in listen_addr;
	memset(&listen_addr, 0, sizeof(listen_addr));
	listen_addr.sin_family = AF_INET;
	listen_addr.sin_addr.s_addr = INADDR_ANY;
	listen_addr.sin_port = htons(port);
	if (bind(listenfd, (struct sockaddr *)&listen_addr, sizeof(listen_addr)) < 0) {
        syslog(LOG_ERR, "bind failed");		
        goto exit;
	}
	if (listen(listenfd, CONNECTION_BACKLOG) < 0) {		
        syslog(LOG_ERR, "listen failed");
        goto exit;
	}
	int reuseaddr_on = 1;
	setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &reuseaddr_on, sizeof(reuseaddr_on));

	/* Set the socket to non-blocking, this is essential in event
	 * based programming with libevent. */
	if (setnonblock(listenfd) < 0) {
        syslog(LOG_ERR, "failed to set server socket to non-blocking");	
        goto exit;
	}
    
    /* Initialize libevent. */
	event_init();
    
    if ((evbase = event_base_new()) == NULL) {
		syslog(LOG_ERR, "Unable to create socket accept event base");
		close(listenfd);
		goto exit;
	}
    
    /* We now have a listening socket, we create a read event to
	 * be notified when a client connects. */
    struct event ev_accept;
	event_set(&ev_accept, listenfd, EV_READ|EV_PERSIST, on_accept, NULL);
	event_base_set(evbase, &ev_accept);
	event_add(&ev_accept, NULL);

    fifo_init();
    
    for(int i=0; i<CONNECTION_BACKLOG; i++)
    {
        pthread_t pth;	
        pthread_create(&pth, NULL, threadFunc, NULL);
    }
    
	syslog(LOG_INFO, "Listen running.\n");

	/* Start the event loop. */
	event_base_dispatch(evbase);

	event_base_free(evbase);
	evbase = NULL;

    fifo_free();
	close(listenfd);
    
 exit:
    syslog(LOG_INFO, "Stoping.");
    closelog();    
    return EXIT_SUCCESS;
}

