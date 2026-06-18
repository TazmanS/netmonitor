#include "stdio.h"
#include "app.h"
#include "system_monitor.h"
#include "network_monitor.h"

void start_app()
{
  printf("\n");
  printf("=====================================\n");
  printf("            NetMonitor\n");
  printf("=====================================\n\n");

  printf("[ System ]\n");

  printf("CPU Temperature : %.1f C\n",
         get_cpu_temperature());

  printf("RAM Usage       : %.1f %%\n",
         get_ram_usage_percent());

  printf("Uptime          : %.0f sec\n",
         get_uptime());

  LoadAverage load_average = get_load_average();

  if (load_average.one_min >= 0)
  {
    printf("\n[ Load Average ]\n");

    printf("1 minute        : %.2f\n",
           load_average.one_min);

    printf("5 minutes       : %.2f\n",
           load_average.five_min);

    printf("15 minutes      : %.2f\n",
           load_average.fifteen_min);
  }

  NetworkStats stats =
      get_network_stats("wlan0");

  printf("\n[ Network: wlan0 ]\n");

  printf("RX Bytes        : %llu\n",
         stats.rx_bytes);

  printf("RX Packets      : %llu\n",
         stats.rx_packets);

  printf("TX Bytes        : %llu\n",
         stats.tx_bytes);

  printf("TX Packets      : %llu\n",
         stats.tx_packets);

  NetworkSpeed speed =
      get_network_speed("wlan0");

  printf("\n[ Network Speed ]\n");

  printf("Download        : %.2f KB/s\n",
         speed.rx_bytes_per_sec / 1024.0);

  printf("Upload          : %.2f KB/s\n",
         speed.tx_bytes_per_sec / 1024.0);

  printf("\n");
}