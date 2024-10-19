#ifndef UTILS_H
#define UTILS_H
#include <Arduino.h>
#include "structs.h"
#include "definitions.h"
#include "configurations.h"
#include "logger.h"
#include "file_operations.h"

status_t monitor_battery(battery_t *current_battery_state);


#endif //UTILS_H