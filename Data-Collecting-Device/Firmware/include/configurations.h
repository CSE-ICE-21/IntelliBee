#ifndef CONFIGURATIONS_H
#define CONFIGURATIONS_H

#define DIR_PATH "/data"

extern const char* ssid;
extern const char* password;

extern const char* ntpServer;
const long  gmtOffset_sec = 0;
const int   daylightOffset_sec = 19800;

extern RTC_DATA_ATTR state_t current_state;

#endif 