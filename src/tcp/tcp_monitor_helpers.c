#include "tcp_monitor_helpers.h"

const char *tcp_state_to_string(const char *state)
{
  if (strcmp(state, "01") == 0)
  {
    return "ESTABLISHED";
  }

  if (strcmp(state, "0A") == 0)
  {
    return "LISTEN";
  }

  if (strcmp(state, "06") == 0)
  {
    return "TIME_WAIT";
  }

  if (strcmp(state, "08") == 0)
  {
    return "CLOSE_WAIT";
  }

  return "UNKNOWN";
}

nm_status_t split_tcp_address(const char *address, char *ip_hex,
                              size_t ip_hex_size, char *port_hex,
                              size_t port_hex_size)
{
  if (address == NULL || ip_hex == NULL || port_hex == NULL)
  {
    return NM_ERROR;
  }

  if (sscanf(address, "%8[^:]:%4s", ip_hex, port_hex) != 2)
  {
    return NM_ERROR;
  }

  ip_hex[ip_hex_size - 1] = '\0';
  port_hex[port_hex_size - 1] = '\0';

  return NM_OK;
}

nm_status_t hex_to_port(const char *port_hex, unsigned int *port)
{
  if (port_hex == NULL || port == NULL)
  {
    return NM_ERROR;
  }

  if (sscanf(port_hex, "%x", port) != 1)
  {
    return NM_ERROR;
  }

  return NM_OK;
}

nm_status_t hex_to_ipv4(const char *hex_ip, char *ip, size_t ip_size)
{
  unsigned int address = 0;

  if (hex_ip == NULL || ip == NULL)
  {
    return NM_ERROR;
  }

  if (sscanf(hex_ip, "%x", &address) != 1)
  {
    return NM_ERROR;
  }

  unsigned int b1 = address & 0xFF;
  unsigned int b2 = (address >> 8) & 0xFF;
  unsigned int b3 = (address >> 16) & 0xFF;
  unsigned int b4 = (address >> 24) & 0xFF;

  snprintf(ip, ip_size, "%u.%u.%u.%u", b1, b2, b3, b4);

  return NM_OK;
}

nm_status_t parse_tcp_address(const char *address, char *ip, size_t ip_size,
                              unsigned int *port)
{
  nm_status_t status = NM_ERROR;

  char ip_hex[9] = {0};
  char port_hex[5] = {0};

  if (address == NULL || ip == NULL || port == NULL)
  {
    goto cleanup;
  }

  if (split_tcp_address(address, ip_hex, sizeof(ip_hex), port_hex,
                        sizeof(port_hex)) != NM_OK)
  {
    goto cleanup;
  }

  if (hex_to_ipv4(ip_hex, ip, ip_size) != NM_OK)
  {
    goto cleanup;
  }

  if (hex_to_port(port_hex, port) != NM_OK)
  {
    goto cleanup;
  }

  status = NM_OK;

cleanup:

  return status;
}
