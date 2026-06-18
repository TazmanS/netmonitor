#include "network_monitor.h"
#include <string.h>
#include <stdio.h>
#include <unistd.h>

NetworkStats get_network_stats(const char *interface_name)
{
  NetworkStats network_stats = {0, 0, 0, 0};

  FILE *file = fopen("/proc/net/dev", "r");

  if (file == NULL)
  {
    return network_stats;
  }

  char line[512];

  while (fgets(line, sizeof(line), file))
  {
    char iface[32];

    unsigned long long rx_bytes;
    unsigned long long rx_packets;

    unsigned long long tx_bytes;
    unsigned long long tx_packets;

    if (sscanf(
            line,
            " %31[^:]: %llu %llu %*u %*u %*u %*u %*u %*u %llu %llu",
            iface,
            &rx_bytes,
            &rx_packets,
            &tx_bytes,
            &tx_packets) == 5)
    {
      if (strcmp(iface, interface_name) == 0)
      {
        network_stats.rx_bytes = rx_bytes;
        network_stats.rx_packets = rx_packets;

        network_stats.tx_bytes = tx_bytes;
        network_stats.tx_packets = tx_packets;

        break;
      }
    }
  }

  fclose(file);

  return network_stats;
}

NetworkSpeed get_network_speed(const char *interface_name)
{
  NetworkSpeed speed = {0};

  NetworkStats first = get_network_stats(interface_name);

  sleep(1);

  NetworkStats second = get_network_stats(interface_name);

  speed.rx_bytes_per_sec = second.rx_bytes - first.rx_bytes;

  speed.tx_bytes_per_sec = second.tx_bytes - first.tx_bytes;

  return speed;
}
