#include "sensor_manipulation.h"

status_t read_env_data(float *temp_in, float *temp_out, float *hum_in, float *hum_out)
{
    digitalWrite(AHT_EN, HIGH);

    TwoWire i2c_in = TwoWire(0);
    TwoWire i2c_out = TwoWire(1);

    i2c_in.begin(SDA_AHT_IN, SCL_AHT_IN, I2C_CLOCK);
    i2c_out.begin(SDA_AHT_OUT, SCL_AHT_OUT, I2C_CLOCK);
    delay(100);

    Adafruit_AHTX0 aht_in;
    Adafruit_AHTX0 aht_out;

    bool aht_found_01 = aht_in.begin(&i2c_in);
    bool aht_found_02 = aht_out.begin(&i2c_out);

    if (!aht_found_01 || !aht_found_02)
    {
        String msg = "Tempurature Sensors are not responding : AHT_IN - " + String(aht_found_01) + " | AHT_OUT - " + String(aht_found_02);
        log_msg(msg.c_str(), "WARNING", "AHT CONNECTIVITY");
        i2c_in.end();
        i2c_out.end();
        digitalWrite(AHT_EN, LOW);
        return ERROR;
    }

    *temp_in = 0, *temp_out = 0, *hum_in = 0, *hum_out = 0;
    sensors_event_t hum_in_event, hum_out_event, temp_in_event, temp_out_event;

    int current_tries = MAX_RETRIES;
    for (int i = 0; i < MEAN_CALCULATION_SIZE; i++)
    {
        bool aht_in_read_status = aht_in.getEvent(&hum_in_event, &temp_in_event);
        bool aht_out_read_status = aht_out.getEvent(&hum_out_event, &temp_out_event);

        if (aht_in_read_status == false || aht_out_read_status == false)
        {
            current_tries--;
            i--;
            if (current_tries <= 0)
            {
                String msg = "Failed to read data from AHT : AHT_IN - " + String(temp_in_event.temperature) + " | " + String(hum_in_event.relative_humidity) + " | AHT_OUT - " + String(temp_out_event.temperature) + " | " + String(hum_out_event.relative_humidity);
                log_msg(msg.c_str(), "ERROR", "AHT DATA LOGGING");
                i2c_in.end();
                i2c_out.end();
                digitalWrite(AHT_EN, LOW);
                return ERROR;
            }
            continue;
        }

        *temp_in += temp_in_event.temperature;
        *temp_out += temp_out_event.temperature;
        *hum_in += hum_in_event.relative_humidity;
        *hum_out += hum_out_event.relative_humidity;
        delay(100);
    }

    *temp_in /= MEAN_CALCULATION_SIZE;
    *temp_out /= MEAN_CALCULATION_SIZE;
    *hum_in /= MEAN_CALCULATION_SIZE;
    *hum_out /= MEAN_CALCULATION_SIZE;

    i2c_in.end();
    i2c_out.end();

    digitalWrite(AHT_EN, LOW);

    return OKAY;
}
