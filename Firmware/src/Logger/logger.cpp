#include "logger.h"

status_t log_mic_data(context_t *device_context)
{
    digitalWrite(2, HIGH);
    int array[1000];
    unsigned long start_time;

    for (int i = 0; i < 1000; i++)
    {
        start_time = micros();
        array[i] = analogRead(MIC_PIN_OUT);
        unsigned long time_taken = micros() - start_time;

        if (1000 > time_taken)   delayMicroseconds(1000 - time_taken);
        yield();
    }

    status_t file_status = appendArray(SD, device_context->file_path.c_str(), array, 1000);
    digitalWrite(2, LOW);
    
    if (file_status == ERROR)
    {
        return ERROR;
    }
    else
    {
        Serial.print("Data Written to : ");
        Serial.println(device_context->file_path);
        return OKAY;
    }
}
