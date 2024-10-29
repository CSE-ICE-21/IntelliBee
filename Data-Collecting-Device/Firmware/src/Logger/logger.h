#ifndef LOGGER_H
#define LOGGER_H

#include <Arduino.h>
#include "definitions.h"
#include "configurations.h"
#include "structs.h"
#include "SD.h"
#include "FileManager/FileManager.h"

status_t log_msg(const char *msg, const char *type, const char *event);
status_t move_logs_from_device_to_sd();
status_t log_csv_data(context_t *device_context);

#endif