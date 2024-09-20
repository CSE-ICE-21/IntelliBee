#ifndef DEFINITIONS_H
#define DEFINITIONS_H
#include "structs.h"

#define SCL_AHT_IN 27
#define SDA_AHT_IN 26

#define SCL_AHT_OUT 33
#define SDA_AHT_OUT 32

#define I2C_CLOCK 400000

#define MIC_PIN_OUT 34
#define MIC_GAIN_PIN 12

#define SD_MOSI     23
#define SD_MISO     19
#define SD_SCLK     18
#define SD_CS       5

#define MAX_RETRIES 5

#define uS_TO_S_FACTOR 1000000  
#define TIME_TO_SLEEP 300 //seconds

#define LED_PIN 2

#define STATES 9

extern RTC_DATA_ATTR state_t current_state;

#endif