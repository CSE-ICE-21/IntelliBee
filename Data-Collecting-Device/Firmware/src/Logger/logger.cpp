#include "logger.h"
#include <stdlib.h>
#include "SPIFFS.h"

RTC_DATA_ATTR int counter_microphone = 0;

status_t log_mic_data(context_t *device_context)
{
    if (counter_microphone % 3 != 0)
    {
        counter_microphone++;
        return OKAY;
    }
    counter_microphone = 1;
    digitalWrite(2, HIGH);
    int16_t *array = (int16_t *)malloc(30000 * sizeof(int16_t));
    if (array == nullptr) // Check if memory allocation failed
    {
        log_msg("Memory Allocation Failed", "ERROR", "MICROPHONE DATA LOGGING");
        return ERROR;
    }

    unsigned long start_time;
    for (int i = 0; i < 30000; i++)
    {
        start_time = micros();
        unsigned long time_taken = micros() - start_time;
        if (time_taken < 1000)
            delayMicroseconds(1000 - time_taken);
    }


    // Free the dynamically allocated memory after use
    free(array);

    digitalWrite(2, LOW);


}

status_t log_csv_data(context_t *device_context)
{
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo))
    {
        // log_msg("Failed access local time", "ERROR", "TIME ACCESS");
        // Serial.println("Failed to get local Time");
        return ERROR;
    }
    String time_stamp = String(timeinfo.tm_year + 1900) + "-" + String(timeinfo.tm_mon + 1) + "-" + String(timeinfo.tm_mday) + " " + String(timeinfo.tm_hour) + ":" + String(timeinfo.tm_min) + ":" + String(timeinfo.tm_sec);
    String csv_data = time_stamp + "," + String(device_context->battery_state->battery_level) + "," + String(device_context->battery_state->charging) + "," + String(device_context->temp_in) + "," + String(device_context->temp_out) + "," + String(device_context->hum_in) + "," + String(device_context->hum_out) + "\n";
    status_t file_status = appendFile(SD, device_context->csv_file_path.c_str(), csv_data.c_str());

    if (file_status == ERROR)
    {
        // log_msg("Failed to write data to file", "ERROR", "AHT DATA LOGGING");
        // Serial.println("Failed to write data to file");
        return ERROR;
    }
    else
    {
        String msg = "Data Written to : " + device_context->csv_file_path;
        // Serial.println(msg);
        // log_msg(msg.c_str(), "INFO", "AHT DATA LOGGING");
        return OKAY;
    }
}

status_t move_logs_from_device_to_sd()
{
    move_indevice_to_sd(SPIFFS, SD, "/logs.txt", "/logs/logs.txt");
    return OKAY;
}

status_t log_msg(const char *msg, const char *type, const char *event)
{

    if (!SPIFFS.begin(true))
    {
        return ERROR;
    }
    if (current_state == SLEEP)
    {
        if (SPIFFS.usedBytes() > 100000)
        {
            move_logs_from_device_to_sd();
        }
    }
    struct tm timeinfo;
    String time_stamp;
    if (getLocalTime(&timeinfo))
    {
        time_stamp = "[" + String(timeinfo.tm_year + 1900) + "-" + String(timeinfo.tm_mon + 1) + "-" + String(timeinfo.tm_mday) + " " + String(timeinfo.tm_hour) + ":" + String(timeinfo.tm_min) + ":" + String(timeinfo.tm_sec) + "] ";
    }
    String log_msg = time_stamp + "[" + type + "] - " + "[" + event + " (" + String(current_state) + ")" + "] " + msg + "\n";
    status_t file_status = appendFile(SPIFFS, "/logs.txt", log_msg.c_str());
    if (file_status == ERROR)
    {
        SPIFFS.end();
    }
    else
    {
        return OKAY;
    }
}
