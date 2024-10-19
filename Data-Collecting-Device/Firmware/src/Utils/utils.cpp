#include "utils.h"

status_t monitor_battery(battery_t *current_battery_state)
{
    digitalWrite(BATTERY_MONITOR_EN, HIGH);
    delay(MAX_LATENCY);

    int battery_level_raw = 0;

    for (int i = 0; i < MEAN_CALCULATION_SIZE; i++)
    {
        battery_level_raw += analogRead(BATTERY_MONITOR_PIN);
        delay(MAX_LATENCY);
    }

    int discharge = 0;

    if(current_battery_state->battery_level != 0)
    {
        discharge = current_battery_state->battery_level - battery_level_raw;
    }

    current_battery_state->battery_level = battery_level_raw/MEAN_CALCULATION_SIZE;
    current_battery_state->charging = digitalRead(CHARG_DETECT);

    status_t record_added = appendFile(SD, DIR_PATH"/battery.csv", String(current_battery_state->battery_level).c_str());

    if (record_added == ERROR)
    {
        log_msg("Failed to write battery data to file", "ERROR", "BATTERY MONITOR");
        digitalWrite(BATTERY_MONITOR_EN, LOW);
        return ERROR;
    }


    if (current_battery_state->battery_level < MINIMUM_BATTERY_LEVEL)
    {
        log_msg("Battery Level is below threshold", "WARNING", "BATTERY MONITOR");
        digitalWrite(BATTERY_MONITOR_EN, LOW);
        return BATTERY_LOW_ERROR;
    }

    digitalWrite(BATTERY_MONITOR_EN, LOW);
    return OKAY;
}