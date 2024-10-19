#ifndef STRUCTS_H
#define STRUCTS_H
#include <Arduino.h>
#include "Adafruit_AHTX0.h"

typedef enum
{
    OKAY = 0,
    ERROR = 1,
    BATTERY_LOW_ERROR = 2,
} status_t;

typedef enum
{
    IDLE = -1,
    WIFI_CONNECTING = 0,
    SYNCRONIZING_TIME = 1,
    SETUP = 2,
    TIME_SYNCRONIZED = 3,
    SPI_INIT = 4,
    SD_INIT = 5,
    SD_MOUNTED = 6,
    LOG_MIC_DATA = 7,
    LOG_AHT_DATA = 8,
    SLEEP = 9,
} state_t;

typedef struct
{
    String file_path;
    String csv_file_path;
    state_t current_state;
    Adafruit_AHTX0 *aht_in;
    Adafruit_AHTX0 *aht_out;
    float temp_in;
    float temp_out;
    float hum_in;
    float hum_out;
} context_t;

typedef struct 
{   
    int battery_level;
    bool charging;

} battery_t;

#endif