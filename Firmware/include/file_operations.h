#ifndef FILE_OPERATIONS_H
#define FILE_OPERATIONS_H
#include "structs.h"
#include "FS.h"

status_t appendFile(fs::FS &fs, const char *path, const char *message);
status_t createDir(fs::FS &fs, const char *path);
status_t writeFile(fs::FS &fs, const char *path, const char *message);
status_t appendArray(fs::FS &fs, const char *path, int16_t *array, int size);
status_t move_indevice_to_sd(fs::FS &fs_indevice,fs::FS &fs_sd,const char * device_path,const char *sd_path);

#endif
