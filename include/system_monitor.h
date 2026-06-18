#ifndef SYSTEM_MONITOR_H
#define SYSTEM_MONITOR_H

#include "common.h"

typedef struct
{
  float one_min;
  float five_min;
  float fifteen_min;
} LoadAverage;

NM_Status get_cpu_temperature(float *temperature);
double get_uptime(void);
float get_ram_usage_percent(void);
LoadAverage get_load_average(void);

#endif