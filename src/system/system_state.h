#ifndef SYSTEM_STATE_H
#define SYSTEM_STATE_H

#include "common.h"

typedef struct
{
  float one_min;
  float five_min;
  float fifteen_min;
} nm_load_average_t;

typedef struct
{
  float temperature;
  double up_time;
  float ram_usage;
  nm_load_average_t load_average;
} nm_system_state_t;

nm_status_t update_system_state(nm_system_state_t *state);

#endif