#ifndef NETWORK_MONITOR_H
#define NETWORK_MONITOR_H

typedef struct
{
  unsigned long long rx_bytes;
  unsigned long long rx_packets;

  unsigned long long tx_bytes;
  unsigned long long tx_packets;
} NetworkStats;

typedef struct
{
  double rx_bytes_per_sec;
  double tx_bytes_per_sec;
} NetworkSpeed;

NetworkStats get_network_stats(const char *interface_name);
NetworkSpeed get_network_speed(const char *interface_name);

#endif