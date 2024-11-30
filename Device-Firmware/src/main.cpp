#include <Arduino.h>
#include "definitions.h"

#include "configurations.h"
#include "time.h"

void setup()
{
  pinMode(LATCH_EN, OUTPUT);
  digitalWrite(LATCH_EN, HIGH);
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(POWER_OFF_PIN, INPUT);
  delay(3000);
  Serial.begin(115200);
}

void loop()
{
  bool power_off = digitalRead(POWER_OFF_PIN);
  if (power_off == 0)
  {
    digitalWrite(RED_LED, HIGH);
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(LATCH_EN, LOW);
  }
  digitalWrite(RED_LED, LOW);
  delay(250);
  digitalWrite(RED_LED, HIGH);
  delay(750);
}
