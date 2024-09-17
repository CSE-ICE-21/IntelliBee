#include "file_operations.h"

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

status_t appendArray(fs::FS &fs, const char *path, int *array, int size)
{
  File file = fs.open(path, FILE_APPEND);
  if (!file)
  {
    return ERROR;
  }
  for (int i = 0; i < size; i++)
  {
    if (file.print(String(array[i]) + ","))
    {
      continue;
    }
    else
    {
      return ERROR;
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
}