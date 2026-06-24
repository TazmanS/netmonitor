#include "app.h"
#include <stdio.h>

static nm_status_t refresh_app_context(app_context_t *context);

static nm_status_t refresh_app_context(app_context_t *context)
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

  if (update_cpu_monitor(&context->cpu_monitor) != NM_OK)
  {
    goto cleanup;
  }

  if (update_tcp_state(&context->tcp_state) != NM_OK)
  {
    goto cleanup;
  }

  status = NM_OK;

cleanup:

  return status;
}

nm_status_t init_app(app_context_t *context)
{
  if (context == NULL)
  {
    return NM_ERROR;
  }

  *context = (app_context_t){0};

  if (get_network_info("wlan0", &context->network_info) != NM_OK)
  {
    return NM_ERROR;
  }

  return refresh_app_context(context);
}

nm_status_t update_app(app_context_t *context)
{
  return refresh_app_context(context);
}

static void format_uptime(double uptime_seconds, char *buffer,
                          size_t buffer_size)
{
  unsigned long uptime = (unsigned long)uptime_seconds;

  unsigned long days = uptime / 86400;
  unsigned long hours = (uptime % 86400) / 3600;
  unsigned long minutes = (uptime % 3600) / 60;
  unsigned long seconds = uptime % 60;

  if (days > 0)
  {
    snprintf(buffer, buffer_size, "%lud %luh %lum %lus", days, hours, minutes,
             seconds);
  }
  else if (hours > 0)
  {
    snprintf(buffer, buffer_size, "%luh %lum %lus", hours, minutes, seconds);
  }
  else if (minutes > 0)
  {
    snprintf(buffer, buffer_size, "%lum %lus", minutes, seconds);
  }
  else
  {
    snprintf(buffer, buffer_size, "%lus", seconds);
  }
}

static void format_bytes(unsigned long long bytes, char *buffer,
                         size_t buffer_size)
{
  const char *units[] = {"B", "KB", "MB", "GB"};

  double value = (double)bytes;
  int unit = 0;

  while (value >= 1024.0 && unit < 3)
  {
    value /= 1024.0;
    unit++;
  }

  snprintf(buffer, buffer_size, "%.2f %s", value, units[unit]);
}

void print_app(app_context_t *context)
{
  char uptime[64];
  char rx_data[32];
  char tx_data[32];

  format_uptime(context->system_state.up_time, uptime, sizeof(uptime));

  format_bytes(context->network_monitor.current.rx_bytes, rx_data,
               sizeof(rx_data));

  format_bytes(context->network_monitor.current.tx_bytes, tx_data,
               sizeof(tx_data));

  printf("\n");
  printf("==================================================\n");
  printf("                    NetMonitor\n");
  printf("==================================================\n");

  printf("\n[ System ]\n");

  printf("%-15s : %.1f C\n", "Temperature", context->system_state.temperature);

  printf("%-15s : %.1f %%\n", "RAM Usage", context->system_state.ram_usage);

  printf("%-15s : %.1f %%\n", "CPU Usage", context->cpu_monitor.usage_percent);

  printf("%-15s : %s\n", "Uptime", uptime);

  printf("\n[ Load Average ]\n");

  printf("%-15s : %.2f\n", "1 min", context->system_state.load_average.one_min);

  printf("%-15s : %.2f\n", "5 min",
         context->system_state.load_average.five_min);

  printf("%-15s : %.2f\n", "15 min",
         context->system_state.load_average.fifteen_min);

  printf("\n[ Network Info ]\n");

  printf("%-15s : %s\n", "Interface", "wlan0");

  printf("%-15s : %s\n", "IP Address", context->network_info.ip_address);

  printf("\n[ Network Statistics ]\n");

  printf("%-15s : %s\n", "RX Data", rx_data);

  printf("%-15s : %llu\n", "RX Packets",
         context->network_monitor.current.rx_packets);

  printf("%-15s : %s\n", "TX Data", tx_data);

  printf("%-15s : %llu\n", "TX Packets",
         context->network_monitor.current.tx_packets);

  printf("\n[ Network Speed ]\n");

  printf("%-15s : %.2f KB/s\n", "Download",
         context->network_monitor.network_speed.rx_bytes_per_sec / 1024.0);

  printf("%-15s : %.2f KB/s\n", "Upload",
         context->network_monitor.network_speed.tx_bytes_per_sec / 1024.0);

  printf("\n[ TCP Connections ]\n");

  printf("%-15s : %u\n", "Count", context->tcp_state.connection_count);

  printf("\n");

  for (unsigned int i = 0; i < context->tcp_state.connection_count; i++)
  {
    printf("%-15s:%-5d -> %-15s:%-5d [%s]\n",
           context->tcp_state.connections[i].local_ip,
           context->tcp_state.connections[i].local_port,
           context->tcp_state.connections[i].remote_ip,
           context->tcp_state.connections[i].remote_port,
           context->tcp_state.connections[i].state);
  }

  printf("\n");
}