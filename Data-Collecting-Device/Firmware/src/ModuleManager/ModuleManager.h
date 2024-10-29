#ifndef MODULE_MANAGER
#define MODULE_MANAGER

#include <SPI.h>
#include "SD.h"
#include "Logger/logger.h"
#include "FileManager/FileManager.h"
#include "Utils/Utils.h"

status_t isSDCardWorking();
status_t isAHTSensorsWorking();
status_t isMICWorking();
status_t init_file_system(context_t *device_context);


#endif //MODULE_MANAGER