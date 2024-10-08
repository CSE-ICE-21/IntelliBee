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


#define BUFFER_SIZE 1024
#define SAMPLE_RATE 8000
#define I2S_MIC_CHANNEL I2S_CHANNEL_FMT_ONLY_LEFT
#define I2S_MIC_SERIAL_CLOCK GPIO_NUM_23
#define I2S_MIC_LEFT_RIGHT_CLOCK GPIO_NUM_25
#define I2S_MIC_SERIAL_DATA GPIO_NUM_22

extern RTC_DATA_ATTR state_t current_state;



#endif