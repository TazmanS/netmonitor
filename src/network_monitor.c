#include "network_monitor.h"
#include <string.h>
#include <stdio.h>

#define NETWORK_STATS_PATH "/proc/net/dev"

static nm_status_t get_network_stats(const char *interface_name, nm_network_stats_t *stats);
static nm_status_t calculate_network_speed(nm_network_monitor_t *monitor);

static nm_status_t get_network_stats(const char *interface_name, nm_network_stats_t *stats)
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

static nm_status_t calculate_network_speed(nm_network_monitor_t *monitor)
{
  nm_status_t status = NM_ERROR;

  if (monitor == NULL)
  {
    goto cleanup;
  }

  monitor->network_speed.rx_bytes_per_sec =
      (double)(monitor->current.rx_bytes -
               monitor->previous.rx_bytes);

  monitor->network_speed.tx_bytes_per_sec =
      (double)(monitor->current.tx_bytes -
               monitor->previous.tx_bytes);
  status = NM_OK;

cleanup:

  return status;
}

nm_status_t update_network_monitor(const char *interface_name, nm_network_monitor_t *monitor)
{
  nm_status_t status = NM_ERROR;

  if (interface_name == NULL || monitor == NULL)
  {
    goto cleanup;
  }

  monitor->previous = monitor->current;

  if (get_network_stats(interface_name, &monitor->current) != NM_OK)
  {
    goto cleanup;
  }

  if (calculate_network_speed(monitor) != NM_OK)
  {
    goto cleanup;
  }

  status = NM_OK;

cleanup:

  return status;
}
