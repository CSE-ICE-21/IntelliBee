#ifndef SENSOR_MANAGER
#define SENSOR_MANAGER

#include <Arduino.h>
#include <Wire.h>
#include <driver/i2s.h>
#include <Adafruit_AHTX0.h>
#include "structs.h"
#include "definitions.h"
#include "configurations.h"
#include "Logger/logger.h"

status_t get_aht_data(context_t *device_context);
status_t write_mic_data(context_t *device_context);
status_t read_mic_data(int32_t *array, int size);   


#endif //SENSOR_MANAGER