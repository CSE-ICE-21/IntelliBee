#include <Arduino.h>
#include <Wire.h>
#include <WiFi.h>
#include "time.h"
#include <SPI.h>
#include <Adafruit_AHTX0.h>
#include "SD.h"
#include "structs.h"
#include "definitions.h"
#include "file_operations.h"
#include "configurations.h"
#include "logger.h"

RTC_DATA_ATTR context_t device_context;
RTC_DATA_ATTR state_t current_state = WIFI_CONNECTING;

TwoWire i2c_01 = TwoWire(0);
TwoWire i2c_02 = TwoWire(1);

Adafruit_AHTX0 aht_in;
Adafruit_AHTX0 aht_out;

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
    log_msg("Connecting to WIFI", "INFO", "WIFI CONNECTION");
    current_tries--;
  }
  if (current_tries == 0)
  {
    log_msg("Failed to establish connection with WIFI", "WARNING", "WIFI CONNECTION");
    return ERROR;
  }
  return OKAY;
}

status_t sync_time(context_t *device_context)
{
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  log_msg("Waiting for time sync", "INFO", "TIME SYNC");
  return OKAY;
}

status_t setup_aht(context_t *device_context)
{
  i2c_01.begin(SDA_AHT_01, SCL_AHT_01, I2C_CLOCK);
  i2c_02.begin(SDA_AHT_02, SCL_AHT_02, I2C_CLOCK);
  bool aht_found_01 = device_context->aht_in->begin(&i2c_01);
  bool aht_found_02 = device_context->aht_out->begin(&i2c_02);
  if (!aht_found_01 || !aht_found_02)
  {
    String msg = "Tempurature Sensors are not responding : AHT_IN - " + String(aht_found_01) + " | AHT_OUT - " + String(aht_found_02);
    log_msg(msg.c_str(), "WARNING", "AHT CONNECTIVITY");
    return ERROR;
  }
  return OKAY;
}

status_t encode_file_path(context_t *device_context)
{
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo))
  {
    log_msg("Failed access local time", "ERROR", "TIME ACCESS");
    return ERROR;
  }
  log_msg("Local time accessed", "INFO", "TIME ACCESS");
  device_context->file_path = String(DIR_PATH) + "/" + String(timeinfo.tm_year + 1900) + "-" + String(timeinfo.tm_mon + 1) + "-" + String(timeinfo.tm_mday) + "-" + String(timeinfo.tm_hour) + "-" + String(timeinfo.tm_min) + "-" + String(timeinfo.tm_sec) + ".txt";
  WiFi.disconnect(true);
  return OKAY;
}

status_t init_SPI(context_t *device_context)
{
  SPI.begin(SD_SCLK, SD_MISO, SD_MOSI, SD_CS);
  log_msg("SPI Initialized", "INFO", "SPI INIT");
  return OKAY;
}

status_t init_sd_card(context_t *device_context)
{
  int current_retries = MAX_RETRIES;
  while (!SD.begin(SD_CS) && current_retries > 0)
  {
    log_msg("Cannot mount SD", "WARNING", "SD MOUNT");
    show_status(800, 200);
    current_retries--;
  }

  if (current_retries == 0)
  {
    log_msg("Failed to mount SD", "ERROR", "SD MOUNT");
    show_status(800, 200);
    return ERROR;
  }

  uint8_t cardType = SD.cardType();

  if (cardType == CARD_NONE)
  {
    log_msg("SD Card type not supported", "WARNING", "SD CARD ACCESS");
    show_status(800, 200);
    return ERROR;
  }
  log_msg("SD Card mounted", "INFO", "SD CARD ACCESS");
  return OKAY;
}

status_t mount_sd(context_t *device_context)
{
  if (createDir(SD, DIR_PATH) == ERROR)
  {
    log_msg("Failed to create a directory", "WARNING", "DIRECTORY MAKING");
    show_status(800, 200);
    return ERROR;
  }
  if (writeFile(SD, device_context->file_path.c_str(), "Starting Collecting Data\n") == ERROR)
  {
    log_msg("Failed to write a file", "WARNING", "DIRECTORY MAKING");
    show_status(800, 200);
    return ERROR;
  }
  log_msg("File created", "INFO", "DIRECTORY MAKING");
  return OKAY;
}

status_t (*state_machine[STATES])(context_t *device_context) = {init_wifi, sync_time, setup_aht, encode_file_path, init_SPI, init_sd_card, mount_sd, log_mic_data, log_aht_data};

status_t run_device(context_t *device_context)
{
  int current_tries = MAX_RETRIES;
  while (current_state != SLEEP)
  {
    Serial.println(current_tries);
    current_tries--;
    Serial.print("Current state : ");
    Serial.println(current_state);
    status_t status = state_machine[current_state](device_context);
    if (status == OKAY)
    {
      current_tries = MAX_RETRIES;
      current_state = (state_t)(current_state + 1);
      log_msg("State executed successfully", "INFO", "STATE EXECUTION");
      show_status(100, 900);
    }
    else if (status == ERROR)
    {
      log_msg("Failed to execute state", "ERROR", "STATE EXECUTION");     
      for (int i = 0; i < (int)current_state; i++)
      {
        show_status(900, 100);
      }
    }
    if (current_tries == 0)
    {
      current_state = SLEEP;
      log_msg("Max retries reached", "CRITICAL", "RETRIES");
      log_msg("Rebooting the device", "CRITICAL", "REBOOT");
      ESP.restart();
    }
  }
  if (current_state == SLEEP)
  {
    log_msg("Going to sleep", "INFO", "SLEEP");
    current_state = SETUP;
    i2c_01.end();
    i2c_02.end();
    SD.end();
    SPI.end();
    esp_deep_sleep_start();
  }
  return OKAY;
}

void get_wakeup_reason(){
  esp_sleep_wakeup_cause_t wakeup_reason;
  wakeup_reason = esp_sleep_get_wakeup_cause();

  switch(wakeup_reason)
  {
    case ESP_SLEEP_WAKEUP_TIMER : log_msg("Wakeup was caused by timer", "INFO", "WAKEUP"); break;
    default : log_msg("Wakeup was not caused by timer", "WARNING", "WAKEUP"); break;
  }
}

void setup()
{
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  pinMode(MIC_PIN_OUT, INPUT);
  get_wakeup_reason();
  device_context.current_state = SPI_INIT;
  device_context.file_path = "/data/sample.txt";
  device_context.csv_file_path = "/data/data.csv";
  device_context.aht_in = &aht_in;
  device_context.aht_out = &aht_out;
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  status_t status = run_device(&device_context);
}

void loop()
{
}
