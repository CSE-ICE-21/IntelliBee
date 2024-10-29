#ifndef STRUCTS_H
#define STRUCTS_H
#include <Arduino.h>

typedef enum
{
    OKAY = 0,
    ERROR = 1,
    BATTERY_LOW_ERROR = 2,
    WIFI_ERROR = 3,
    SD_INITIALIZATION_ERROR = 4,
} status_t;

typedef enum
{
    IDLE = -1,
    SYNC_TIME = 0,
    BATTERY_MONITORING = 1,
    ENCODE_FILE_PATH = 2,
    GET_AHT_DATA = 3,
    FILE_SYSTEM_INIT = 4,
    SAVE_CSV_DATA = 5,
    LOG_MIC_DATA = 6,
    SLEEP = 7,
} state_t;

typedef struct 
{   
    int battery_level;
    bool charging;

} battery_t;

typedef struct
{
    String file_path;
    String csv_file_path;
    state_t current_state;
    float temp_in;
    float temp_out;
    float hum_in;
    float hum_out;
    battery_t *battery_state;
} context_t;


#endif