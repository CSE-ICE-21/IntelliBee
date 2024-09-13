#include <Arduino.h>
#include "definitions.h"
#include <unity.h>
#include <Adafruit_AHTX0.h>
#include <Wire.h>
#include <SPI.h>


TwoWire i2c_01 = TwoWire(0);
TwoWire i2c_02 = TwoWire(1);

Adafruit_AHTX0 aht_01;
Adafruit_AHTX0 aht_02;

sensors_event_t humidity, temp;

void setUp(void)
{
}

void tearDown(void)
{
}


// Test function to check I2C initialization
void test_i2c_initialization(void)

{
    bool i2c_01_initialized = i2c_01.begin(SDA_AHT_01,SCL_AHT_01,I2C_CLOCK);
    bool i2c_02_initialized = i2c_02.begin(SDA_AHT_02,SCL_AHT_02,I2C_CLOCK);
    TEST_ASSERT_EQUAL(true, i2c_01_initialized); 
    TEST_ASSERT_EQUAL(true, i2c_02_initialized);
}

void test_aht_sensor(void)
{
    bool aht_01_found = aht_01.begin(&i2c_01);
    bool aht_02_found = aht_02.begin(&i2c_02);
    TEST_ASSERT_EQUAL(true,aht_01_found);
    TEST_ASSERT_EQUAL(true,aht_02_found);
}
void test_get_event(void)
{
    int event = aht_01.getEvent(&humidity,&temp);
    TEST_ASSERT_NOT_EQUAL(0xFF,event);
    TEST_ASSERT_NOT_NULL(&humidity);
    TEST_ASSERT_NOT_NULL(&temp);
}

void test_tempurature_and_humidity(void)
{
    float tempurature = temp.temperature;
    TEST_ASSERT_FLOAT_IS_NOT_NAN(tempurature);
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