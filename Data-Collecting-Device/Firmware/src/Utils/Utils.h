#ifndef UTILS_H
#define UTILS_H

#include <WiFi.h>
#include "time.h"
#include "Logger/logger.h"
#include "FileManager/FileManager.h"

status_t monitor_battery(context_t *device_context);
status_t sync_time(context_t *device_context);
status_t encode_file_path(context_t *device_context);

void show_status(int delay_01, int delay_02);
void init_device(context_t *device_context);

#endif //UTILS_H