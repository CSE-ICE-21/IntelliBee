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

status_t read_mic_data(int32_t *array, int size)
{
    digitalWrite(MIC_EN, HIGH);

    i2s_config_t i2s_config = {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
        .sample_rate = 44100,
        .bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT,
        .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
        .communication_format = I2S_COMM_FORMAT_I2S,
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
        .dma_buf_count = 4,
        .dma_buf_len = 1024,
        .use_apll = false,
        .tx_desc_auto_clear = false,
        .fixed_mclk = 0
        };

    i2s_pin_config_t i2s_mic_pins = {
    .bck_io_num = MIC_SCK,
    .ws_io_num = MIC_WS,
    .data_out_num = I2S_PIN_NO_CHANGE,
    .data_in_num = MIC_SD,
    };

    i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
    i2s_set_pin(I2S_NUM_0, &i2s_mic_pins);

    size_t bytes_read = 0;
    i2s_read(I2S_NUM_0, array, size * sizeof(int32_t), &bytes_read, portMAX_DELAY);
    
    i2s_driver_uninstall(I2S_NUM_0);
    digitalWrite(MIC_EN, LOW);
    
}