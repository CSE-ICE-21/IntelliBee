#ifndef DEFINITIONS_H
#define DEFINITIONS_H
#include "structs.h"

#define AHT_EN 5
#define MIC_EN 23
#define GSM_EN 13
#define BATTERY_MONITOR_EN 2
#define LATCH_EN 4

#define RED_LED 18
#define GREEN_LED 19

#define POWER_OFF_PIN 14

#define BATTERY_MONITOR_PIN 34

#define SCL_AHT_IN 27
#define SDA_AHT_IN 26

#define SCL_AHT_OUT 22
#define SDA_AHT_OUT 21

#define MIC_WS 25
#define MIC_SCK 32
#define MIC_SD 33

#define GSM_TX 17
#define GSM_RX 16

#define MAX_RETRIES 5

#define MEAN_CALCULATION_SIZE 10

#define MAX_LATENCY 100

#define DIFFERENCE 2
#define ZERO_PERCENT 835

#define MINIMUM_BATTERY_LEVEL 10

#define I2C_CLOCK 400000

#define uS_TO_S_FACTOR 1000000

#define TIME_TO_SLEEP 300 // seconds

#define STATES 7

#define TINY_GSM_MODEM_SIM900 // Modem is SIM900
#define SerialMon Serial
#define TINY_GSM_DEBUG SerialMon

class Context
{
public:
    float temp_in;
    float temp_out;
    float hum_in;
    float hum_out;
    battery_t *battery_state;
    state_t current_state;
};

class Manager
{
public:
    Context *context;
    virtual void execute(); // Marked virtual.
    virtual ~Manager() {}   // Virtual destructor.
    void show_error()
    {
        digitalWrite(RED_LED, HIGH);
        delay(1000);
        digitalWrite(RED_LED, LOW);
        delay(2000);
    }
    void show_success()
    {
        digitalWrite(GREEN_LED, HIGH);
        delay(2000);
        digitalWrite(GREEN_LED, LOW);
        delay(1000);
    }
};

#endif