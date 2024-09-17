#ifndef STRUCTS_H
#define STRUCTS_H
#include <Arduino.h>

typedef enum
{
    OKAY = 0,
    ERROR = 1
} status_t;

typedef enum
{
    IDLE = -1,
    WIFI_CONNECTING = 0,
    SYNCRONIZING_TIME = 1,
    TIME_SYNCRONIZED = 2,
    SPI_INIT = 3,
    SD_INIT = 4,
    SD_MOUNTED = 5,
    LOG_MIC_DATA = 6,
    SLEEP = 7,
} state_t;

typedef struct
{
    String file_path;
    state_t current_state;
} context_t;

#endif