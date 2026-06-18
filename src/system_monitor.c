#include "system_monitor.h"
#include <stdio.h>
#include <string.h>

float get_cpu_temperature(void)
{
  FILE *file = fopen("/sys/class/thermal/thermal_zone0/temp", "r");

  if (file == NULL)
  {
    return -1.0f;
  }

  int temp_milli_celsius;

  if (fscanf(file, "%d", &temp_milli_celsius) != 1)
  {
    fclose(file);
    return -1.0f;
  }

  fclose(file);

  return temp_milli_celsius / 1000.0f;
}

double get_uptime(void)
{
  FILE *file = fopen("/proc/uptime", "r");

  if (file == NULL)
  {
    return 0;
  }

  double up_time;

  if (fscanf(file, "%lf", &up_time) != 1)
  {
    fclose(file);
    return 0;
  }

  fclose(file);
  return up_time;
}

float get_ram_usage_percent(void)
{
  FILE *file = fopen("/proc/meminfo", "r");

  if (file == NULL)
  {
    return -1.0f;
  }

  char line[256];

  long mem_total = 0;
  long mem_available = 0;

  while (fgets(line, sizeof(line), file))
  {
    if (strncmp(line, "MemTotal:", 9) == 0)
    {
      sscanf(line, "MemTotal: %ld", &mem_total);
    }
    else if (strncmp(line, "MemAvailable:", 13) == 0)
    {
      sscanf(line, "MemAvailable: %ld", &mem_available);
    }

    if (mem_total > 0 && mem_available > 0)
    {
      break;
    }
  }

  fclose(file);

  if (mem_total == 0)
  {
    return -1.0f;
  }

  long used = mem_total - mem_available;

  return (float)used / mem_total * 100.0f;
}

LoadAverage get_load_average(void)
{
  LoadAverage load = {
      -1.0f,
      -1.0f,
      -1.0f};
  FILE *file = fopen("/proc/loadavg", "r");

  if (file == NULL)
  {
    return load;
  }

  LoadAverage load_average = {
      0.0f,
      0.0f,
      0.0f};

  if (fscanf(file, "%f %f %f",
             &load_average.one_min,
             &load_average.five_min,
             &load_average.fifteen_min) != 3)
  {
    fclose(file);
    return load;
  }

  fclose(file);
  return load_average;
}