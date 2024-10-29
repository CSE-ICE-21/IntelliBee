#ifndef DEVICE_MANAGER
#define DEVICE_MANAGER

#include "Logger/logger.h"
#include "FileManager/FileManager.h"
#include "ModuleManager/ModuleManager.h"
#include "SensorManager/SensorManager.h"
#include "Utils/Utils.h"

extern status_t (*state_machine[STATES])(context_t *device_context);

status_t run_device(context_t *device_context);
status_t self_test(context_t *device_context);

#endif //DEVICE_MANAGER