#ifndef CPU_MONITOR_H
#define CPU_MONITOR_H

#include "common.h"

typedef struct
{
  unsigned long long total;
  unsigned long long idle;
} nm_cpu_times_t;

typedef struct
{
  nm_cpu_times_t previous;
  nm_cpu_times_t current;

  float usage_percent;
} nm_cpu_monitor_t;

nm_status_t update_cpu_monitor(nm_cpu_monitor_t *monitor);

#endif