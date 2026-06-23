#include "cpu_monitor.h"

#include <string.h>
#include <stdio.h>

#define CPU_MONITOR_PATH "/proc/stat"

static nm_status_t get_cpu_times(nm_cpu_times_t *times);

static nm_status_t get_cpu_times(nm_cpu_times_t *times)
{
  nm_status_t status = NM_ERROR;
  FILE *file = NULL;

  char cpu_label[8];
  unsigned long long user = 0;
  unsigned long long nice = 0;
  unsigned long long system = 0;
  unsigned long long idle = 0;
  unsigned long long iowait = 0;
  unsigned long long irq = 0;
  unsigned long long softirq = 0;

  if (times == NULL)
  {
    goto cleanup;
  }

  *times = (nm_cpu_times_t){0};

  file = fopen(CPU_MONITOR_PATH, "r");

  if (file == NULL)
  {
    goto cleanup;
  }

  if (fscanf(file,
             "%7s %llu %llu %llu %llu %llu %llu %llu",
             cpu_label,
             &user,
             &nice,
             &system,
             &idle,
             &iowait,
             &irq,
             &softirq) != 8)
  {
    goto cleanup;
  }

  times->total = user + nice + system + idle + iowait + irq + softirq;
  times->idle = idle + iowait;

  status = NM_OK;
cleanup:
  if (file != NULL)
  {
    fclose(file);
  }

  return status;
}

nm_status_t update_cpu_monitor(nm_cpu_monitor_t *monitor)
{
  nm_status_t status = NM_ERROR;

  if (monitor == NULL)
  {
    goto cleanup;
  }

  monitor->previous = monitor->current;

  if (get_cpu_times(&monitor->current) != NM_OK)
  {
    goto cleanup;
  }

  unsigned long long total_delta = monitor->current.total - monitor->previous.total;

  unsigned long long idle_delta = monitor->current.idle - monitor->previous.idle;

  if (total_delta == 0)
  {
    goto cleanup;
  }

  monitor->usage_percent = (1.0f - ((float)idle_delta / (float)total_delta)) * 100.0f;

  status = NM_OK;

cleanup:

  return status;
}
