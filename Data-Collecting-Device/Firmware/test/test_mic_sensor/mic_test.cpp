#include <Arduino.h>
#include <unity.h>
#include "definitions.h"
#include <Arduino.h>
#include "definitions.h"
#include <unity.h>
#include <Adafruit_AHTX0.h>
#include <Wire.h>
#include <SPI.h>


void setUp(void)
{
}

void tearDown(void)
{
}

void test_mic_sensor(void)
{
    TEST_ASSERT_EQUAL(0, digitalRead(MIC_PIN_OUT));
}

void setup()
{
    UNITY_BEGIN(); // IMPORTANT LINE!
    pinMode(MIC_PIN_OUT, INPUT);
    RUN_TEST(test_mic_sensor);
    UNITY_END(); // stop unit testing
}

void loop()
{
}