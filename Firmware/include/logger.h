#ifndef LOGGER_H
#define LOGGER_H
#include <Arduino.h>
#include "definitions.h"
#include "structs.h"
#include "file_operations.h"
#include "SD.h"

status_t log_mic_data(context_t *device_context); 
status_t log_aht_data(context_t *device_context);  
status_t log_msg(const char *msg, const char *type, const char *event);
status_t move_logs_from_device_to_sd();



#endif