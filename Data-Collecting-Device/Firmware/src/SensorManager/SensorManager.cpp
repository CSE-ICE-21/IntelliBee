#include "SensorManager.h"

#define SAMPLE_BUFFER_SIZE 8096
#define SAMPLE_RATE 8000

TwoWire i2c_in = TwoWire(0);
TwoWire i2c_out = TwoWire(1);

Adafruit_AHTX0 aht_in;
Adafruit_AHTX0 aht_out;

i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
    .sample_rate = SAMPLE_RATE,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT,
    .channel_format = I2S_CHANNEL_FMT_ONLY_RIGHT,
    .communication_format =  I2S_COMM_FORMAT_STAND_I2S,
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = 4,
    .dma_buf_len = 1024,
    .use_apll = false,
    .tx_desc_auto_clear = false,
    .fixed_mclk = 0};

// and don't mess around with this
i2s_pin_config_t i2s_mic_pins = {
    .bck_io_num = MIC_SCK,
    .ws_io_num = MIC_WS,
    .data_out_num = I2S_PIN_NO_CHANGE,
    .data_in_num = MIC_SD
    };

void deinit_aht_sensors()
{
    i2c_in.end();
    i2c_out.end();
    digitalWrite(AHT_EN, LOW);
}

status_t init_aht_sensors()
{
    digitalWrite(AHT_EN, HIGH);
    i2c_in.begin(SDA_AHT_IN, SCL_AHT_IN, I2C_CLOCK);
    i2c_out.begin(SDA_AHT_OUT, SCL_AHT_OUT, I2C_CLOCK);
    delay(100);

    bool aht_found_01 = aht_in.begin(&i2c_in);
    bool aht_found_02 = aht_out.begin(&i2c_out);

    if (!aht_found_01 || !aht_found_02)
    {
        String msg = "Tempurature Sensors are not responding : AHT_IN - " + String(aht_found_01) + " | AHT_OUT - " + String(aht_found_02);
        log_msg(msg.c_str(), "WARNING", "AHT CONNECTIVITY");
        deinit_aht_sensors();
        return ERROR;
    }

    return OKAY;
}

status_t init_mic_sensor()
{
    digitalWrite(MIC_EN, HIGH);
    esp_err_t err = i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
    if (err != ESP_OK)
    {
        // Serial.println("Failed to install I2S driver");
        // log_msg("Failed to install I2S driver", "ERROR", "MIC INIT");
        digitalWrite(MIC_EN, LOW);
        return ERROR;
    }
    err = i2s_set_pin(I2S_NUM_0, &i2s_mic_pins);
    if (err != ESP_OK)
    {
        // Serial.println("Failed to set I2S pins");
        // log_msg("Failed to set I2S pins", "ERROR", "MIC INIT");
        digitalWrite(MIC_EN, LOW);
        return ERROR;
    }
    return OKAY;
}

void deinit_mic_sensor()
{
    i2s_driver_uninstall(I2S_NUM_0);
    digitalWrite(MIC_EN, LOW);
}

status_t read_env_data(float *temp_in, float *temp_out, float *hum_in, float *hum_out)
{
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
                // Serial.println(msg);
                // log_msg(msg.c_str(), "ERROR", "AHT DATA LOGGING");
                deinit_aht_sensors();
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

    return OKAY;
}

status_t get_aht_data(context_t *device_context)
{
    status_t status = init_aht_sensors();
    if (status != OKAY)
    {
        // Serial.println("AHT initialization failed !");
        return ERROR;
    }

    status = read_env_data(&device_context->temp_in, &device_context->temp_out, &device_context->hum_in, &device_context->hum_out);
    if (status != OKAY)
    {
        // Serial.println("AHT Reading Failed");
        return ERROR;
    }
    // Serial.print("Inside Tempuratures : ");
    // Serial.println(device_context->temp_in);
    // Serial.print("Outside Tempuratures : ");
    // Serial.println(device_context->temp_out);
    // Serial.print("Inside Humidity : ");
    // Serial.println(device_context->hum_in);
    // Serial.print("Outside Tempuratures : ");
    // Serial.println(device_context->hum_out);
    deinit_aht_sensors();
    return OKAY;
}

status_t read_mic_data(int32_t *array, int size)
{
    size_t bytes_read = 0;
    esp_err_t err;
    for (int i = 0; i < 5; i++)
    {
        err = i2s_read(I2S_NUM_0, array, size * sizeof(int32_t), &bytes_read, portMAX_DELAY);
        if (err != ESP_OK)
            return ERROR;
        int zeros = 0;
        for (int i = 0; i < 8096; i++)
        {
            if (array[i] == 0)
                zeros++;
        }
        // Serial.print("Zeros : ");
        // Serial.println(zeros);
    }
    // Serial.println("Bytes Read : ");
    // Serial.println(bytes_read);
    return OKAY;
}

status_t write_mic_data(context_t *device_context)
{
    status_t status = init_mic_sensor();
    if (status != OKAY)
    {
        // Serial.println("MIC initialization failed !");
        deinit_mic_sensor();
        return ERROR;
    }

    int32_t *raw_samples = (int32_t *)malloc(sizeof(int32_t) * SAMPLE_BUFFER_SIZE);

    for(int i = 0; i < 5; i++)
    {
        size_t bytes_read = 0;
        i2s_read(I2S_NUM_0, raw_samples, sizeof(int32_t) * SAMPLE_BUFFER_SIZE, &bytes_read, portMAX_DELAY);
        int samples_read = bytes_read / sizeof(int32_t);
        int zeros = 0;
        for (int i = 0; i < samples_read; i++)
        {
            raw_samples[i] = raw_samples[i]/256;
            if (raw_samples[i] == 0)
                zeros++;
        }
    }

    status = appendArray(SD, device_context->file_path.c_str(), raw_samples, SAMPLE_BUFFER_SIZE);
    free(raw_samples);
    if(status != OKAY)
    {
        // Serial.println("Failed to write mic data to SD card");
        deinit_mic_sensor();
        return ERROR;
    }
    // Serial.println("Mic data written to SD card");
    return OKAY;
}
