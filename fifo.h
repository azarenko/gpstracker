/*
 * FIFO buffer
 */
#include <pthread.h>

#define BUFSIZE 1024
int fifobuf[BUFSIZE];
int *pIn, *pOut, *pEnd;
int full;
pthread_mutex_t lock;

int isfifofull();

// init
void fifo_init();

// add char 'c' to buffer
int fifo_put(int val);

// get a char from circular buffer
int fifo_get(int *val);

void fifo_free();
