#include "tcp_monitor.h"
#include "tcp_monitor_helpers.h"

#define TCP_STATE_PATH "/proc/net/tcp"

nm_status_t update_tcp_state(nm_tcp_state_t *state)
{
  FILE *file = NULL;
  nm_status_t status = NM_ERROR;
  char line[512];

  if (state == NULL)
  {
    goto cleanup;
  }

  *state = (nm_tcp_state_t){0};

  file = fopen(TCP_STATE_PATH, "r");

  if (file == NULL)
  {
    goto cleanup;
  }

  if (fgets(line, sizeof(line), file) == NULL)
  {
    goto cleanup;
  }

  unsigned int index = 0;

  while (fgets(line, sizeof(line), file) && index < NM_MAX_TCP_CONNECTIONS)
  {
    char local_address[32] = {0};
    char remote_address[32] = {0};
    char state_code[4] = {0};

    nm_tcp_connection_t *connection = &state->connections[index];

    if (sscanf(line, "%*d: %31s %31s %3s", local_address, remote_address,
               state_code) != 3)
    {
      continue;
    }

    if (parse_tcp_address(local_address, connection->local_ip,
                          sizeof(connection->local_ip),
                          &connection->local_port) != NM_OK)
    {
      continue;
    }

    if (parse_tcp_address(remote_address, connection->remote_ip,
                          sizeof(connection->remote_ip),
                          &connection->remote_port) != NM_OK)
    {
      continue;
    }

    snprintf(connection->state, sizeof(connection->state), "%s",
             tcp_state_to_string(state_code));

    index++;
  }

  state->connection_count = index;

  status = NM_OK;

cleanup:
  if (file != NULL)
  {
    fclose(file);
  }

  return status;
}
