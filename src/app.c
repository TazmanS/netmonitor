#include "stdio.h"
#include "app.h"
#include "system_monitor.h"
#include "network_monitor.h"

void start_app(void)
{
  float cpu_temperature = 0.0f;
  float ram_usage = 0.0f;
  double uptime = 0.0;

  nm_load_average_t load_average = {0};

  nm_network_stats_t stats = {0};
  nm_network_speed_t speed = {0};

  get_cpu_temperature(&cpu_temperature);
  get_ram_usage_percent(&ram_usage);
  get_uptime(&uptime);

  get_load_average(&load_average);

  get_network_stats(
      "wlan0",
      &stats);

  get_network_speed(
      "wlan0",
      &speed);

  printf("\n");
  printf("=====================================\n");
  printf("            NetMonitor\n");
  printf("=====================================\n\n");

  printf("[ System ]\n");

  printf("CPU Temperature : %.1f C\n",
         cpu_temperature);

  printf("RAM Usage       : %.1f %%\n",
         ram_usage);

  printf("Uptime          : %.0f sec\n",
         uptime);

  printf("\n[ Load Average ]\n");

  printf("1 minute        : %.2f\n",
         load_average.one_min);

  printf("5 minutes       : %.2f\n",
         load_average.five_min);

  printf("15 minutes      : %.2f\n",
         load_average.fifteen_min);

  printf("\n[ Network: wlan0 ]\n");

  printf("RX Bytes        : %llu\n",
         stats.rx_bytes);

  printf("RX Packets      : %llu\n",
         stats.rx_packets);

  printf("TX Bytes        : %llu\n",
         stats.tx_bytes);

  printf("TX Packets      : %llu\n",
         stats.tx_packets);

  printf("\n[ Network Speed ]\n");

  printf("Download        : %.2f KB/s\n",
         speed.rx_bytes_per_sec / 1024.0);

  printf("Upload          : %.2f KB/s\n",
         speed.tx_bytes_per_sec / 1024.0);

  printf("\n");
}