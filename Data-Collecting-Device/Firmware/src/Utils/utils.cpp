#include "Utils/Utils.h"
#include "time.h"

const char *ssid = "Dialog 4G 058";
const char *password = "9c60Dee1";
const char *ntpServer = "pool.ntp.org";

void show_status(int delay_01, int delay_02)
{
  digitalWrite(BUILTIN_LED, HIGH);
  delay(delay_01);
  digitalWrite(BUILTIN_LED, LOW);
  delay(delay_02);
}

status_t init_wifi()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  int current_tries = MAX_RETRIES;
  while (WiFi.status() != WL_CONNECTED && current_tries > 0)
  {
    show_status(500, 500);
    // Serial.println("Connecting to WiFi");
    // log_msg("Connecting to WIFI", "INFO", "WIFI CONNECTION");
    current_tries--;
    if (current_tries == 0)
    {
      // log_msg("Failed to establish connection with WIFI", "WARNING", "WIFI CONNECTION");
      return WIFI_ERROR;
    }
  }
  // Serial.println("WiFi Connected");
  return OKAY;
}

void deinit_wifi()
{
  WiFi.disconnect(true);
}

status_t sync_time(context_t *device_context)
{
  status_t status = init_wifi();
  if (status != OKAY)
    log_msg("Failed to establish connection with WIFI", "WARNING", "WIFI CONNECTION");
  delay(5000);
  configTime(0, 19800, ntpServer);
  delay(5000);
    struct tm timeinfo;
  if (!getLocalTime(&timeinfo))
  {
    // log_msg("Failed access local time", "ERROR", "TIME ACCESS");
    // Serial.println("Failed to get local Time in sync");
    return ERROR;
  }
  deinit_wifi();
  return OKAY;
}

status_t encode_file_path(context_t *device_context)
{
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo))
  {
    // log_msg("Failed access local time", "ERROR", "TIME ACCESS");
    // Serial.println("Failed to get local Time");
    return ERROR;
  }
  device_context->file_path = String(DIR_PATH) + "/" + String(timeinfo.tm_year + 1900) + "-" + String(timeinfo.tm_mon + 1) + "-" + String(timeinfo.tm_mday) + "-" + String(timeinfo.tm_hour) + "-" + String(timeinfo.tm_min) + "-" + String(timeinfo.tm_sec) + ".txt";
  // Serial.print("File Path : ");
  // Serial.println(device_context->file_path);
  return OKAY;
}

status_t monitor_battery(context_t *device_context)
{
  // Serial.println("Reading Battery Level");
  digitalWrite(BATTERY_MONITOR_EN, HIGH);
  delay(MAX_LATENCY);
  // Serial.print("Last Battery Level : ");
  // Serial.println(device_context->battery_state->battery_level);
  int battery_level_raw = 0;

  for (int i = 0; i < MEAN_CALCULATION_SIZE; i++)
  {
    battery_level_raw += analogRead(BATTERY_MONITOR_PIN);
    delay(MAX_LATENCY);
  }

  int discharge = 0;
  int lastCharge = device_context->battery_state->battery_level;
  int battery_level = ((battery_level_raw / MEAN_CALCULATION_SIZE) - ZERO_PERCENT)/DIFFERENCE;
  // Serial.print("Battery Level : ");
  // Serial.println(battery_level);
  device_context->battery_state->battery_level = battery_level;
  device_context->battery_state->charging = digitalRead(CHARG_DETECT);

  if (lastCharge != 0)
  {
    discharge = lastCharge - device_context->battery_state->battery_level;
  }

  if (discharge < 0)
    device_context->battery_state->battery_level = lastCharge;

  if (device_context->battery_state->battery_level < MINIMUM_BATTERY_LEVEL)
  {
    log_msg("Battery Level is below threshold", "WARNING", "BATTERY MONITOR");
    digitalWrite(BATTERY_MONITOR_EN, LOW);\
    gpio_hold_dis(GPIO_NUM_4);
    digitalWrite(BATTERY_MONITOR_EN,LOW);
    return BATTERY_LOW_ERROR;
  }
  // Serial.print("Battery Level : ");
  // Serial.println(device_context->battery_state->battery_level);
  // Serial.print("Discharge : ");
  // Serial.println(discharge);
  // Serial.print("Is Charging : ");
  // Serial.println(digitalRead(CHARG_DETECT));

  digitalWrite(BATTERY_MONITOR_EN, LOW);
  return OKAY;
}

void get_wakeup_reason()
{
  esp_sleep_wakeup_cause_t wakeup_reason;
  wakeup_reason = esp_sleep_get_wakeup_cause();

  switch (wakeup_reason)
  {
  case ESP_SLEEP_WAKEUP_TIMER:
    log_msg("Wakeup was caused by timer", "INFO", "WAKEUP");
    break;
  default:
    log_msg("Wakeup was not caused by timer", "WARNING", "WAKEUP");
    break;
  }
}

void init_device(context_t *device_context)
{
  pinMode(BATTERY_MONITOR_EN, OUTPUT);
  pinMode(AHT_EN, OUTPUT);
  pinMode(MIC_EN, OUTPUT);
  pinMode(SD_EN, OUTPUT);

  pinMode(CHARG_DETECT, INPUT);
  pinMode(BATTERY_MONITOR_PIN, INPUT);
}