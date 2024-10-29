#include "DeviceManager.h"

status_t (*state_machine[STATES])(context_t *device_context) = {sync_time, monitor_battery, encode_file_path, get_aht_data, init_file_system, log_csv_data, write_mic_data};

RTC_DATA_ATTR state_t current_state = SYNC_TIME;

status_t run_device(context_t *device_context)
{
    Serial.println("Hello");
    int current_tries = MAX_RETRIES;
    while (current_state != SLEEP)
    {
        current_tries--;
        status_t status = state_machine[current_state](device_context);
        if (status != OKAY)
        {
            Serial.print("Failed to execute state : ");
            Serial.println(current_state);
            for (int i = 0; i < (int)current_state; i++)
            {
                show_status(900, 100);
            }
        }
        else
        {
            show_status(100, 900);
            current_tries = MAX_RETRIES;
            current_state = (state_t)(current_state + 1);
            Serial.println("State executed successfully");
        }
        if (current_tries == 0)
        {
            current_state = SLEEP;
            // log_msg("Max retries reached", "CRITICAL", "RETRIES");
            // log_msg("Rebooting the device", "CRITICAL", "REBOOT");
            ESP.restart();
        }
    }
    if (current_state == SLEEP)
    {
        // Serial.println("Going to sleep");
        current_state = BATTERY_MONITORING;
        show_status(1000, 1000);
        esp_deep_sleep_start();
    }
    return OKAY;
}
