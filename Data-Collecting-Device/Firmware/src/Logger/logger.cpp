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
        array[i] = analogRead(MIC_PIN_OUT);
        unsigned long time_taken = micros() - start_time;
        if (time_taken < 1000)
            delayMicroseconds(1000 - time_taken);
    }

    status_t file_status = appendArray(SD, device_context->file_path.c_str(), array, 30000);

    // Free the dynamically allocated memory after use
    free(array);

    digitalWrite(2, LOW);

    // Check if the file write was successful
    if (file_status == ERROR)
    {
        log_msg("Failed to write data to file", "ERROR", "MICROPHONE DATA LOGGING");
        return ERROR;
    }
    else
    {
        String msg = "Data Written to : " + device_context->file_path;
        log_msg(msg.c_str(), "INFO", "MICROPHONE DATA LOGGING");
        return OKAY;
    }
}

status_t log_aht_data(context_t *device_context)
{
    digitalWrite(2, HIGH);
    sensors_event_t hum_in, hum_out, temp_in, temp_out;
    device_context->aht_in->getEvent(&hum_in, &temp_in);
    device_context->aht_out->getEvent(&hum_out, &temp_out);
    digitalWrite(2, LOW);

    if (isnan(hum_in.relative_humidity) || isnan(temp_in.temperature) || isnan(hum_out.relative_humidity) || isnan(temp_out.temperature))
    {
        String msg = "Failed to read data from AHT : AHT_IN - " + String(temp_in.temperature) + " | " + String(hum_in.relative_humidity) + " | AHT_OUT - " + String(temp_out.temperature) + " | " + String(hum_out.relative_humidity);
        log_msg(msg.c_str(), "ERROR", "AHT DATA LOGGING");
        return ERROR;
    }

    String csv_data = String(temp_in.temperature) + "," + String(hum_in.relative_humidity) + "," + String(temp_out.temperature) + "," + String(hum_out.relative_humidity) + "\n";

    status_t file_status = appendFile(SD, device_context->csv_file_path.c_str(), csv_data.c_str());

    if (file_status == ERROR)
    {
        log_msg("Failed to write data to file", "ERROR", "AHT DATA LOGGING");
        return ERROR;
    }
    else
    {
        String msg = "Data Written to : " + device_context->csv_file_path;
        log_msg(msg.c_str(), "INFO", "AHT DATA LOGGING");
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
