#include "ModuleManager.h"

void init_SPI()
{
  SPI.begin(SD_SCLK, SD_MISO, SD_MOSI, SD_CS);
  // Serial.println("SPI Initialized");
  // log_msg("SPI Initialized", "INFO", "SPI INIT");
}

void deinit_sd_card()
{
  SD.end();
  SPI.end();
  digitalWrite(SD_EN, LOW);
}

status_t init_sd_card()
{
  digitalWrite(SD_EN, HIGH);
  init_SPI();
  int current_retries = MAX_RETRIES;
  while (!SD.begin(SD_CS) && current_retries > 0)
  {
    // log_msg("Cannot mount SD", "WARNING", "SD MOUNT");
    // Serial.println("Cannot mount SD");
    show_status(800, 200);
    delay(1000);
    current_retries--;
    if (current_retries == 0)
    {
      // log_msg("Failed to mount SD", "ERROR", "SD MOUNT");
      // Serial.println("Failed to mount SD");
      show_status(800, 200);
      return ERROR;
    }
  }

  uint8_t cardType = SD.cardType();

  if (cardType == CARD_NONE)
  {
    // log_msg("SD Card type not supported", "WARNING", "SD CARD ACCESS");
    // Serial.println("SD Card type not supporte");
    show_status(800, 200);
    return ERROR;
  }
  // log_msg("SD Card mounted", "INFO", "SD CARD ACCESS");
  // Serial.println("SD Card Mounted");
  return OKAY;
}

status_t mount_sd(context_t *device_context)
{
  if (createDir(SD, DIR_PATH) == ERROR)
  {
    // log_msg("Failed to create a directory", "WARNING", "DIRECTORY MAKING");
    // Serial.println("Failed to create a DIR");
    show_status(800, 200);
    return ERROR;
  }
  if (writeFile(SD, device_context->file_path.c_str(), "Starting Collecting Data\n") == ERROR)
  {
    log_msg("Failed to write a file", "WARNING", "DIRECTORY MAKING");
    show_status(800, 200);
    return ERROR;
  }
  // log_msg("File created", "INFO", "DIRECTORY MAKING");
  // Serial.println("File Created");
  return OKAY;
}

status_t init_file_system(context_t *device_context)
{
  status_t status = init_sd_card();
  if (status != OKAY)
  {
    deinit_sd_card();
    return ERROR;
  }
  status = mount_sd(device_context);
  if (status != OKAY)
  {
    deinit_sd_card();
    return ERROR;
  }

  return OKAY;
}