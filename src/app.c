#include "stdio.h"
#include "app.h"
#include "system_state.h"
#include "network_monitor.h"

nm_status_t init_app(app_context_t *context)
{
  nm_status_t status = NM_ERROR;

  if (context == NULL)
  {
    goto cleanup;
  }

  *context = (app_context_t){0};

  if (update_network_monitor("wlan0", &context->network_monitor) != NM_OK)
  {
    goto cleanup;
  }

  status = NM_OK;

cleanup:

  return status;
}

nm_status_t update_app(app_context_t *context)
{
  nm_status_t status = NM_ERROR;
  if (context == NULL)
  {
    goto cleanup;
  }

  if (update_network_monitor("wlan0", &context->network_monitor) != NM_OK)
  {
    goto cleanup;
  }

  if (update_system_state(&context->system_state) != NM_OK)
  {
    goto cleanup;
  }

  status = NM_OK;

cleanup:
  return status;
}

void print_app(app_context_t *context)
{
  printf("\n");
  printf("=====================================\n");
  printf("            NetMonitor\n");
  printf("=====================================\n\n");

  printf("[ System ]\n");

  printf("CPU Temperature : %.1f C\n", context->system_state.temperature);

  printf("RAM Usage       : %.1f %%\n", context->system_state.ram_usage);

  printf("Uptime          : %.0f sec\n", context->system_state.up_time);

  printf("\n[ Load Average ]\n");

  printf("1 minute        : %.2f\n", context->system_state.load_average.one_min);

  printf("5 minutes       : %.2f\n", context->system_state.load_average.five_min);

  printf("15 minutes      : %.2f\n", context->system_state.load_average.fifteen_min);

  printf("\n[ Network: wlan0 ]\n");

  printf("RX Bytes        : %llu\n", context->network_monitor.current.rx_bytes);

  printf("RX Packets      : %llu\n", context->network_monitor.current.rx_packets);

  printf("TX Bytes        : %llu\n", context->network_monitor.current.tx_bytes);

  printf("TX Packets      : %llu\n", context->network_monitor.current.tx_packets);

  printf("\n[ Network Speed ]\n");

  printf("Download        : %.2f KB/s\n", context->network_monitor.network_speed.rx_bytes_per_sec / 1024.0);

  printf("Upload          : %.2f KB/s\n", context->network_monitor.network_speed.tx_bytes_per_sec / 1024.0);

  printf("\n");
}