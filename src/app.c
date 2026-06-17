#include "stdio.h"
#include "app.h"
#include "system_monitor.h"

void start_app()
{
  printf("Start APP\n");
  printf("CPU Temperature: %.1f\n", get_cpu_temperature());
  printf("UP Time: %.2lf\n", get_uptime());
}