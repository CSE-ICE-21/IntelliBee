#include <Arduino.h>
#include <unity.h>
// #include "definitions.h"
#include <driver/i2s.h>
#include <Wire.h>
#include <SPI.h>

#define SAMPLE_BUFFER_SIZE 512
#define SAMPLE_RATE 8000
// most microphones will probably default to left channel but you may need to tie the L/R pin low
#define I2S_MIC_CHANNEL I2S_CHANNEL_FMT_ONLY_LEFT
// either wire your microphone to the same pins or change these to match your wiring
#define I2S_MIC_SERIAL_CLOCK GPIO_NUM_23
#define I2S_MIC_LEFT_RIGHT_CLOCK GPIO_NUM_25
#define I2S_MIC_SERIAL_DATA GPIO_NUM_22

i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
    .sample_rate = SAMPLE_RATE,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT,
    .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
    .communication_format = I2S_COMM_FORMAT_I2S,
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = 4,
    .dma_buf_len = 1024,
    .use_apll = false,
    .tx_desc_auto_clear = false,
    .fixed_mclk = 0};

i2s_pin_config_t i2s_mic_pins = {
    .bck_io_num = I2S_MIC_SERIAL_CLOCK,
    .ws_io_num = I2S_MIC_LEFT_RIGHT_CLOCK,
    .data_out_num = I2S_PIN_NO_CHANGE,
    .data_in_num = I2S_MIC_SERIAL_DATA};

int32_t raw_samples[SAMPLE_BUFFER_SIZE];

void setUp(void)
{
}

void tearDown(void)
{
}

void test_i2s_driver_install(void)
{
    esp_err_t i2s_driver_installed = i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
    TEST_ASSERT_EQUAL(ESP_OK, i2s_driver_installed);
}

void test_i2s_set_pin(void)
{
    esp_err_t i2s_pin_set = i2s_set_pin(I2S_NUM_0, &i2s_mic_pins);
    TEST_ASSERT_EQUAL(ESP_OK, i2s_pin_set);
}

void test_i2s_read(void)
{
    size_t bytes_read = 0;
    i2s_read(I2S_NUM_0, raw_samples, sizeof(int32_t) * SAMPLE_BUFFER_SIZE, &bytes_read, portMAX_DELAY);
    TEST_ASSERT_NOT_EQUAL(0, bytes_read);
    Serial.println("Bytes read: " + String(bytes_read));
   
}

void setup()
{
    UNITY_BEGIN(); // IMPORTANT LINE!
    RUN_TEST(test_i2s_driver_install);
    RUN_TEST(test_i2s_set_pin);
    RUN_TEST(test_i2s_read);
    UNITY_END(); // stop unit testing
}



void loop()
{
    
}