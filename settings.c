#include "settings.h"

int readSettings(const char* configFilePath)
{
    config_t cfg;
    config_setting_t *setting;
    const char *str;

    config_init(&cfg);
  
    /* Read the file. If there is an error, report it and exit. */
    if(!config_read_file(&cfg, configFilePath))
    {
        fprintf(stderr, "%s:%d - %s\n", config_error_file(&cfg),
                config_error_line(&cfg), config_error_text(&cfg));
        config_destroy(&cfg);
        return(EXIT_FAILURE);
    }
    
    config_destroy(&cfg);    
    return EXIT_SUCCESS;
}
