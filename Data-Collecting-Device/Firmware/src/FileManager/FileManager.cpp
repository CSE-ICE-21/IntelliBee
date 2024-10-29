#include "FileManager.h"

status_t appendFile(fs::FS &fs, const char *path, const char *message)
{

  File file = fs.open(path, FILE_APPEND);
  if (!file)
  {
    return ERROR;
  }
  if (!file.print(message))
  {
    return ERROR;
  }
  file.close();
  return OKAY;
}

status_t appendArray(fs::FS &fs, const char *path, int32_t *array, int size)
{
  File file = fs.open(path, FILE_APPEND);
  if (!file)
  {
    return ERROR;
  }
  for (int j = 0; j < size; j = j + 1000)
  {
    for (int i = 0; i < 1000; i++)
    {
      if (file.print(String(array[i + j]) + ","))
      {
        continue;
      }
      else
      {
        return ERROR;
      }
    }
  }
  file.close();
  return OKAY;
}

status_t createDir(fs::FS &fs, const char *path)
{
  if (fs.mkdir(path))
  {
    return OKAY;
  }
  else
  {
    return ERROR;
  }
}

status_t writeFile(fs::FS &fs, const char *path, const char *message)
{

  File file = fs.open(path, FILE_WRITE);
  if (!file)
  {
    return ERROR;
  }
  if (file.print(message))
  {
    return OKAY;
  }
  else
  {
    return ERROR;
  }
  file.close();
  return OKAY;
}

status_t move_indevice_to_sd(fs::FS &fs_indevice, fs::FS &fs_sd, const char *device_path, const char *sd_path)
{

  File file_device = fs_indevice.open(device_path);
  if (!file_device)
  {
    return ERROR;
  }
  if (!fs_sd.exists(sd_path))
  {
    writeFile(fs_sd, sd_path, "\n");
  }
  File file_sd = fs_sd.open(sd_path, FILE_APPEND);
  if (!file_device)
  {
    return ERROR;
  }

  uint8_t buffer[512];
  size_t bytes_read;

  while ((bytes_read = file_device.read(buffer, sizeof(buffer))) > 0)
  {
    file_sd.write(buffer, bytes_read);
  }

  file_sd.close();
  file_device.close();
  fs_indevice.remove(device_path);
  return OKAY;
}