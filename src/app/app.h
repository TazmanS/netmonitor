#ifndef APP_H
#define APP_H

#include "../network/network_monitor.h"
#include "../system/system_state.h"
#include "../cpu/cpu_monitor.h"
#include "../tcp/tcp_monitor.h"
#include "../network/network_info.h"

typedef struct
{
  nm_network_monitor_t network_monitor;
  nm_system_state_t system_state;
  nm_cpu_monitor_t cpu_monitor;
  nm_tcp_state_t tcp_state;
  nm_network_info_t network_info;
} app_context_t;

void print_app(app_context_t *context);
nm_status_t init_app(app_context_t *context);
nm_status_t update_app(app_context_t *context);

#endif