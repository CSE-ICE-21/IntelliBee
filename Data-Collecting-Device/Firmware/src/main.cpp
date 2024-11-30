#include <Arduino.h>
#include "DeviceManager/DeviceManager.h"




RTC_DATA_ATTR battery_t current_battery_state = {.battery_level = 0,.charging=false};
RTC_DATA_ATTR context_t device_context = {"/data", "/data/data.csv", BATTERY_MONITORING, 0, 0, 0, 0, &current_battery_state};


void setup()
{
    pinMode(LATCH_EN, OUTPUT);
    digitalWrite(LATCH_EN, HIGH);
    gpio_hold_en(GPIO_NUM_4);
    init_device(&device_context);

    esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
    Serial.begin(115200);
    run_device(&device_context);
    Serial.begin(115200, SERIAL_8N1, 16,17);
}

void loop()
{
}
