#ifndef SENSOR_MANIPULATION
#define SENSOR_MANIPULATION

#include <Arduino.h>
#include <Wire.h>
#include <driver/i2s.h>
#include <Adafruit_AHTX0.h>
#include "structs.h"
#include "definitions.h"
#include "configurations.h"
#include "logger.h"


status_t read_env_data(float *temp_in, float *temp_out, float *hum_in, float *hum_out);
status_t read_mic_data(int16_t *array, int size);   


#endif //SENSOR_MANIPULATION