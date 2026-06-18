#include "stdio.h"
#include "app.h"
#include "system_monitor.h"

void start_app()
{
  printf("Start APP\n");

  printf("CPU Temperature: %.1f\n", get_cpu_temperature());
  printf("UP Time: %.2lf\n", get_uptime());
  printf("RAM Usage percent: %.1f%%\n", get_ram_usage_percent());

  LoadAverage load_average = get_load_average();

  if (load_average.one_min > 0 && load_average.five_min > 0 && load_average.fifteen_min > 0)
  {
    printf("Load Average 1 min: %.1f\n", load_average.one_min);
    printf("Load Average 5 min: %.1f\n", load_average.five_min);
    printf("Load Average 15 min: %.1f\n", load_average.fifteen_min);
  }
}