#ifndef SYSTEM_MONITOR_H
#define SYSTEM_MONITOR_H

#include "common.h"

typedef struct
{
  float one_min;
  float five_min;
  float fifteen_min;
} nm_load_average_t;

nm_status_t get_cpu_temperature(float *temperature);
nm_status_t get_uptime(double *up_time);
nm_status_t get_ram_usage_percent(float *ram_usage);
nm_status_t get_load_average(nm_load_average_t *load_average);

#endif