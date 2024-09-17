#ifndef LOGGER_H
#define LOGGER_H
#include <Arduino.h>
#include "definitions.h"
#include "structs.h"
#include "file_operations.h"
#include "SD.h"

status_t log_mic_data(context_t *device_context);   

#endif