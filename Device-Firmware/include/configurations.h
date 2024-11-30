#ifndef __CONFIGURATIONS_H__
#define __CONFIGURATIONS_H__
#include "structs.h"

#define DIR_PATH "/data"

#define MQTT_SERVER "test.mosquitto.org"
#define MQTT_PORT 1883
#define MQTT_DEVICE_ID "IntelliBee-Device-01"
#define DATA_RECEPTION_TOPIC "IntelliBee-Reception"
#define DATA_SENDING_TOPIC "IntelliBee-Sending"

extern const char* ssid;
extern const char* password;

const long  gmtOffset_sec = 0;
const int  daylightOffset_sec = 19800;

extern const char *apn;
extern const char *ntpServer;

extern RTC_DATA_ATTR state_t current_state;

#endif 