#include "system_state.h"
#include <stdio.h>
#include <string.h>

#define CPU_TEMPERATURE_PATH "/sys/class/thermal/thermal_zone0/temp"
#define UPTIME_PATH "/proc/uptime"
#define RAM_USAGE_PATH "/proc/meminfo"
#define LOAD_AVERAGE_PATH "/proc/loadavg"

#define MEM_TOTAL "MemTotal:"
#define MEM_AVAILABLE "MemAvailable:"

static nm_status_t get_cpu_temperature(float *temperature);
static nm_status_t get_uptime(double *up_time);
static nm_status_t get_ram_usage_percent(float *ram_usage);
static nm_status_t get_load_average(nm_load_average_t *load_average);

static nm_status_t get_cpu_temperature(float *temperature)
{
  nm_status_t status = NM_ERROR;
  FILE *file = NULL;
  int temp_milli_celsius = 0;

  if (temperature == NULL)
  {
    goto cleanup;
  }

  *temperature = 0.0f;

  file = fopen(CPU_TEMPERATURE_PATH, "r");

  if (file == NULL)
  {
    goto cleanup;
  }

  if (fscanf(file, "%d", &temp_milli_celsius) != 1)
  {
    goto cleanup;
  }

  *temperature = (float)temp_milli_celsius / 1000.0f;

  status = NM_OK;

cleanup:
  if (file != NULL)
  {
    fclose(file);
  }

  return status;
}

static nm_status_t get_uptime(double *up_time)
{
  nm_status_t status = NM_ERROR;
  FILE *file = NULL;

  if (up_time == NULL)
  {
    goto cleanup;
  }

  *up_time = 0.0;
  file = fopen(UPTIME_PATH, "r");

  if (file == NULL)
  {
    goto cleanup;
  }

  if (fscanf(file, "%lf", up_time) != 1)
  {
    goto cleanup;
  }

  status = NM_OK;

cleanup:
  if (file != NULL)
  {
    fclose(file);
  }

  return status;
}

static nm_status_t get_ram_usage_percent(float *ram_usage)
{
  FILE *file = NULL;
  nm_status_t status = NM_ERROR;

  char line[256];
  long mem_total = 0;
  long mem_available = 0;

  if (ram_usage == NULL)
  {
    goto cleanup;
  }

  *ram_usage = 0.0f;

  file = fopen(RAM_USAGE_PATH, "r");
  if (file == NULL)
  {
    goto cleanup;
  }

  while (fgets(line, sizeof(line), file))
  {
    if (strncmp(line, MEM_TOTAL, strlen(MEM_TOTAL)) == 0)
    {
      if (sscanf(line, MEM_TOTAL " %ld", &mem_total) != 1)
      {
        goto cleanup;
      }
    }
    else if (strncmp(line, MEM_AVAILABLE, strlen(MEM_AVAILABLE)) == 0)
    {
      if (sscanf(line, MEM_AVAILABLE " %ld", &mem_available) != 1)
      {
        goto cleanup;
      }
    }

    if (mem_total > 0 && mem_available > 0)
    {
      break;
    }
  }

  if (mem_total == 0 || mem_available == 0)
  {
    goto cleanup;
  }

  *ram_usage = (float)(mem_total - mem_available) / mem_total * 100.0f;

  status = NM_OK;

cleanup:
  if (file != NULL)
  {
    fclose(file);
  }

  return status;
}

static nm_status_t get_load_average(nm_load_average_t *load_average)
{
  nm_status_t status = NM_ERROR;
  FILE *file = NULL;

  if (load_average == NULL)
  {
    goto cleanup;
  }

  *load_average = (nm_load_average_t){
      0.0f,
      0.0f,
      0.0f};

  file = fopen(LOAD_AVERAGE_PATH, "r");

  if (file == NULL)
  {
    goto cleanup;
  }

  if (fscanf(file, "%f %f %f",
             &load_average->one_min,
             &load_average->five_min,
             &load_average->fifteen_min) != 3)
  {
    goto cleanup;
  }

  status = NM_OK;

cleanup:
  if (file != NULL)
  {
    fclose(file);
  }

  return status;
}

nm_status_t update_system_state(nm_system_state_t *state)
{
  nm_status_t status = NM_ERROR;

  if (state == NULL)
  {
    goto cleanup;
  }

  if (get_cpu_temperature(&state->temperature) != NM_OK)
  {
    goto cleanup;
  }

  if (get_uptime(&state->up_time) != NM_OK)
  {
    goto cleanup;
  }

  if (get_ram_usage_percent(&state->ram_usage) != NM_OK)
  {
    goto cleanup;
  }

  if (get_load_average(&state->load_average) != NM_OK)
  {
    goto cleanup;
  }

  status = NM_OK;
cleanup:
  return status;
}