#include "devices.h"

void *threadFunc(void *arg)
{      
    while(1)
    {
        int client_fd;
        if(fifo_get(&client_fd) == EXIT_FAILURE)
        {
            usleep(100 * 1000);
            continue;
        }
          
        proto(&client_fd);    
        close(client_fd);
    }    
}
