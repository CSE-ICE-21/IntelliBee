#include <Arduino.h>
#include <Wire.h>
#include <WiFi.h>
#include "time.h"
#include <SPI.h>
#include "SD.h"
#include "structs.h"
#include "definitions.h"
#include "file_operations.h"
#include "configurations.h"
#include "logger.h"


#define CONFIG_ESP_INT_WDT_TIMEOUT_MS 1000
#define CONFIG_ESP_TASK_WDT_TIMEOUT_S 60

RTC_DATA_ATTR context_t device_context;
RTC_DATA_ATTR state_t current_state = WIFI_CONNECTING;


void show_status(int daly_01, int delay_02)
{
  digitalWrite(2, HIGH);
  delay(daly_01);
  digitalWrite(2, LOW);
  delay(delay_02);
}

status_t init_wifi(context_t *device_context)
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  int current_tries = MAX_RETRIES;
  while (WiFi.status() != WL_CONNECTED && current_tries > 0)
  {
    show_status(500, 500);
    current_tries--;
  }
  if (current_tries == 0)
    return ERROR;
  return OKAY;
}

status_t sync_time(context_t *device_context)
{
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  return OKAY;
}

status_t encode_file_path(context_t *device_context)
{
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo))
  {
    return ERROR;
  }
  device_context->file_path = String(DIR_PATH) + "/" + String(timeinfo.tm_year + 1900) + "-" + String(timeinfo.tm_mon + 1) + "-" + String(timeinfo.tm_mday) + "-" + String(timeinfo.tm_hour) + "-" + String(timeinfo.tm_min) + "-" + String(timeinfo.tm_sec) + ".txt";
  WiFi.disconnect(true);
  return OKAY;
}

status_t init_SPI(context_t *device_context)
{
  SPI.begin(SD_SCLK, SD_MISO, SD_MOSI, SD_CS);
  return OKAY;
}

status_t init_sd_card(context_t *device_context)
{
  int current_retries = MAX_RETRIES;
  while (!SD.begin(SD_CS) && current_retries > 0)
  {
    show_status(500, 500);
    current_retries--;
  }

  if (current_retries == 0)
  {
    return ERROR;
  }

  uint8_t cardType = SD.cardType();

  if (cardType == CARD_NONE)
  {
    return ERROR;
  }
  return OKAY;
}

status_t mount_sd(context_t *device_context)
{
  if (createDir(SD, DIR_PATH) == ERROR)
  {
    show_status(1000, 200);
    return ERROR;
  }
  if (writeFile(SD, device_context->file_path.c_str(), "Starting Collecting Data\n") == ERROR)
  {
    return ERROR;
  }
  return OKAY;
}

status_t (*state_machine[STATES])(context_t *device_context) = {init_wifi, sync_time, encode_file_path, init_SPI, init_sd_card, mount_sd,log_mic_data};

status_t run_device(context_t *device_context)
{
  while (current_state != SLEEP)
  {
    Serial.print("Current state : ");
    Serial.println(current_state);
    status_t status = state_machine[current_state](device_context);
    if (status == OKAY)
    {
      //add modulo operation here
      current_state = (state_t)(current_state + 1);
      show_status(100, 900);
    }
    else if (status == ERROR)
    {
      for (int i = 0; i < (int)current_state; i++)
      {
        show_status(900, 100);
      }
    }
  }
  if (current_state == SLEEP)
  {
    current_state = TIME_SYNCRONIZED;
    SD.end();
    SPI.end();
    esp_deep_sleep_start();
  }
  return OKAY;
}

void setup()
{
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  pinMode(MIC_PIN_OUT, INPUT);
  device_context.current_state = SPI_INIT;
  device_context.file_path = "/data/sample.txt";
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  status_t status = run_device(&device_context);
}

void loop()
{
}
