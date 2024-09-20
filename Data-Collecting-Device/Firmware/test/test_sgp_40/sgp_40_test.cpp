#include <Arduino.h>
#include <Adafruit_SGP40.h>
#include <Wire.h>
#include <unity.h>
#include "definitions.h"


TwoWire i2c = TwoWire(0);
Adafruit_SGP40 sgp;

void setUp(void)
{
}

void tearDown(void)
{
}

void test_i2c_initialization(void)
{
    bool i2c_initialized = i2c.begin(SDA_AHT_02,SCL_AHT_02,I2C_CLOCK);
    TEST_ASSERT_EQUAL(true, i2c_initialized);
}

void test_sgp_sensor(void)
{
    bool sgp_found = sgp.begin(&i2c);
    TEST_ASSERT_EQUAL(true,sgp_found);
}

void setup()
{
    UNITY_BEGIN(); // IMPORTANT LINE!
    RUN_TEST(test_i2c_initialization);
    RUN_TEST(test_sgp_sensor);
    UNITY_END(); // stop unit testing
}

void loop()
{
}