#ifndef APP_H
#define APP_H

#include "network_monitor.h"
#include "system_state.h"
#include "cpu_monitor.h"

typedef struct
{
  nm_network_monitor_t network_monitor;
  nm_system_state_t system_state;
  nm_cpu_monitor_t cpu_monitor;
} app_context_t;

void print_app(app_context_t *context);
nm_status_t init_app(app_context_t *context);
nm_status_t update_app(app_context_t *context);

#endif