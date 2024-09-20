#include <Arduino.h>
#include "definitions.h"
#include <unity.h>
#include <Adafruit_AHTX0.h>
#include <Wire.h>
#include <SPI.h>


TwoWire i2c_01 = TwoWire(0);
// TwoWire i2c_02 = TwoWire(1);

Adafruit_AHTX0 aht_01;
// Adafruit_AHTX0 aht_02;

sensors_event_t humidity_01,humidity_02, temp_01,temp_02;

void setUp(void)
{
}

void tearDown(void)
{
}


// Test function to check I2C initialization
void test_i2c_initialization(void)

{
    bool i2c_01_initialized = i2c_01.begin(SDA_AHT_OUT,SCL_AHT_OUT,I2C_CLOCK);
    // bool i2c_02_initialized = i2c_02.begin(SDA_AHT_02,SCL_AHT_02,I2C_CLOCK);
    TEST_ASSERT_EQUAL(true, i2c_01_initialized); 
    // TEST_ASSERT_EQUAL(true, i2c_02_initialized);
}

void test_aht_sensor(void)
{
    bool aht_01_found = aht_01.begin(&i2c_01);
    // bool aht_02_found = aht_02.begin(&i2c_02);
    TEST_ASSERT_EQUAL(true,aht_01_found);
    // TEST_ASSERT_EQUAL(true,aht_02_found);
}
void test_get_event(void)
{
    // int event_02 = aht_02.getEvent(&humidity_02,&temp_02);
    // TEST_ASSERT_NOT_EQUAL(0xFF,event_02);
    // TEST_ASSERT_NOT_NULL(&humidity_02);
    // TEST_ASSERT_NOT_NULL(&temp_02);
    int event_01 = aht_01.getEvent(&humidity_01,&temp_01);
    TEST_ASSERT_NOT_EQUAL(0xFF,event_01);
    TEST_ASSERT_NOT_NULL(&humidity_01);
    TEST_ASSERT_NOT_NULL(&temp_01);
}

void test_tempurature_and_humidity(void)
{
    float tempurature_01 = temp_01.temperature;
    TEST_ASSERT_FLOAT_IS_NOT_NAN(tempurature_01);
    // float tempurature_02 = temp_02.temperature;
    // TEST_ASSERT_FLOAT_IS_NOT_NAN(tempurature_02);
}

void setup()
{
    UNITY_BEGIN(); // IMPORTANT LINE!
    RUN_TEST(test_i2c_initialization);
    RUN_TEST(test_aht_sensor);
    RUN_TEST(test_get_event);
    RUN_TEST(test_tempurature_and_humidity);
    UNITY_END();
}

void loop()
{
}