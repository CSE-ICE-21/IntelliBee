#ifndef DEFINITIONS_H
#define DEFINITIONS_H
#include "structs.h"

#define AHT_EN 17
#define MIC_EN 16
#define SD_EN 13
#define BATTERY_MONITOR_EN 2
#define LATCH_EN 4

#define CHARG_DETECT 35

#define BATTERY_MONITOR_PIN 34

#define SCL_AHT_IN 27
#define SDA_AHT_IN 26

#define SCL_AHT_OUT 22
#define SDA_AHT_OUT 21

#define MIC_WS 25
#define MIC_SCK 32
#define MIC_SD 33

#define SD_MOSI     23
#define SD_MISO     19
#define SD_SCLK     18
#define SD_CS       5

#define MAX_RETRIES 5

#define MEAN_CALCULATION_SIZE 10

#define MAX_LATENCY 100

#define DIFFERENCE 2
#define ZERO_PERCENT 835

#define MINIMUM_BATTERY_LEVEL 10

#define I2C_CLOCK 400000


#define uS_TO_S_FACTOR 1000000  

#define TIME_TO_SLEEP 300 //seconds

#define LED_PIN 2

#define STATES 7


#endif