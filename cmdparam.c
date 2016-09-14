#include "cmdparam.h"

int getCmdParam(int argc, char *argv[], char* name, const char** outvalue)
{
    for(int i=1; i<argc; i++)
    {        
        if(strcmp(argv[i], name) == 0 && i < argc - 1)
        {
            *outvalue = argv[i + 1];
            return EXIT_SUCCESS;
        }
    }
    
    return EXIT_FAILURE;
}
