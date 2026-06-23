#ifndef TCP_MONITOR_H
#define TCP_MONITOR_H

#include "common.h"
#include <stdio.h>
#include <string.h>

#define NM_MAX_TCP_CONNECTIONS 64

typedef struct
{
  char local_ip[16];
  unsigned int local_port;

  char remote_ip[16];
  unsigned int remote_port;

  char state[16];
} nm_tcp_connection_t;

typedef struct
{
  nm_tcp_connection_t connections[NM_MAX_TCP_CONNECTIONS];

  unsigned int connection_count;
} nm_tcp_state_t;

nm_status_t update_tcp_state(nm_tcp_state_t *state);

#endif