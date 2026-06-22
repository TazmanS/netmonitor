#ifndef NETWORK_MONITOR_H
#define NETWORK_MONITOR_H

#include "common.h"

typedef struct
{
  unsigned long long rx_bytes;
  unsigned long long rx_packets;

  unsigned long long tx_bytes;
  unsigned long long tx_packets;
} nm_network_stats_t;

typedef struct
{
  double rx_bytes_per_sec;
  double tx_bytes_per_sec;
} nm_network_speed_t;

typedef struct
{
  nm_network_stats_t previous;
  nm_network_stats_t current;
  nm_network_speed_t network_speed;
} nm_network_monitor_t;

nm_status_t update_network_monitor(const char *interface_name, nm_network_monitor_t *monitor);

#endif