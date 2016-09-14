#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <libconfig.h>

/* 
 *listen address 
 */
const char* addr;

/* 
 *listen port 
 */
int port;

int readSettings(const char* configFilePath);
