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

nm_status_t get_network_stats(const char *interface_name, nm_network_stats_t *stats);
nm_status_t get_network_speed(const char *interface_name, nm_network_speed_t *speed);

#endif