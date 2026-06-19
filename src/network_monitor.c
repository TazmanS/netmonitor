#include "network_monitor.h"
#include <string.h>
#include <stdio.h>
#include <unistd.h>

#define NETWORK_STATS_PATH "/proc/net/dev"

nm_status_t get_network_stats(const char *interface_name, nm_network_stats_t *stats)
{
  nm_status_t status = NM_ERROR;
  FILE *file = NULL;
  char line[512];

  if (interface_name == NULL || stats == NULL)
  {
    goto cleanup;
  }

  *stats = (nm_network_stats_t){0, 0, 0, 0};

  file = fopen(NETWORK_STATS_PATH, "r");

  if (file == NULL)
  {
    goto cleanup;
  }

  while (fgets(line, sizeof(line), file))
  {
    char iface[32] = {0};

    unsigned long long rx_bytes = 0;
    unsigned long long rx_packets = 0;
    unsigned long long tx_bytes = 0;
    unsigned long long tx_packets = 0;

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
        stats->rx_bytes = rx_bytes;
        stats->rx_packets = rx_packets;

        stats->tx_bytes = tx_bytes;
        stats->tx_packets = tx_packets;

        status = NM_OK;

        goto cleanup;
      }
    }
  }

cleanup:
  if (file != NULL)
  {
    fclose(file);
  }

  return status;
}

nm_status_t get_network_speed(const char *interface_name, nm_network_speed_t *speed)
{
  nm_status_t status = NM_ERROR;

  nm_network_stats_t first = {0};
  nm_network_stats_t second = {0};

  if (interface_name == NULL || speed == NULL)
  {
    goto cleanup;
  }

  *speed = (nm_network_speed_t){0.0, 0.0};

  if (get_network_stats(interface_name, &first) != NM_OK)
  {
    goto cleanup;
  }

  sleep(1);

  if (get_network_stats(interface_name, &second) != NM_OK)
  {
    goto cleanup;
  }

  speed->rx_bytes_per_sec = (double)(second.rx_bytes - first.rx_bytes);

  speed->tx_bytes_per_sec = (double)(second.tx_bytes - first.tx_bytes);

  status = NM_OK;

cleanup:

  return status;
}