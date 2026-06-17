#include "system_monitor.h"
#include "stdio.h"

float get_cpu_temperature()
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