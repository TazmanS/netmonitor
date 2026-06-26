#include "network_info.h"

#include <arpa/inet.h>
#include <net/if.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <unistd.h>

#define GATEWAY_PATH "/proc/net/route"

// ***********************STATIC***************************************************

static nm_status_t get_network_ip_address(const char *interface_name,
                                          char *ip_address);
static nm_status_t get_network_mask(const char *interface_name, char *mask);
static nm_status_t get_network_gateway(const char *interface_name,
                                       char *gateway);
static nm_status_t get_network_mac_address(const char *interface_name,
                                           char *mac_address);
static nm_status_t get_network_mtu(const char *interface_name, int *mtu);

// ********************************************************************************

static nm_status_t get_network_ip_address(const char *interface_name,
                                          char *ip_address)
{
  nm_status_t status = NM_ERROR;

  int socket_fd = -1;
  struct ifreq ifr;

  if (interface_name == NULL || ip_address == NULL)
  {
    goto cleanup;
  }

  ifr = (struct ifreq){0};

  socket_fd = socket(AF_INET, SOCK_DGRAM, 0);

  if (socket_fd < 0)
  {
    goto cleanup;
  }

  strncpy(ifr.ifr_name, interface_name, IFNAMSIZ - 1);

  if (ioctl(socket_fd, SIOCGIFADDR, &ifr) < 0)
  {
    goto cleanup;
  }

  struct sockaddr_in *address = (struct sockaddr_in *)&ifr.ifr_addr;

  if (inet_ntop(AF_INET, &address->sin_addr, ip_address, INET_ADDRSTRLEN) ==
      NULL)
  {
    goto cleanup;
  }

  status = NM_OK;

cleanup:
  if (socket_fd >= 0)
  {
    close(socket_fd);
  }

  return status;
}

static nm_status_t get_network_mask(const char *interface_name, char *mask)
{
  nm_status_t status = NM_ERROR;

  int socket_fd = -1;
  struct ifreq ifr;

  if (mask == NULL || interface_name == NULL)
  {
    goto cleanup;
  }

  ifr = (struct ifreq){0};

  socket_fd = socket(AF_INET, SOCK_DGRAM, 0);

  if (socket_fd < 0)
  {
    goto cleanup;
  }

  strncpy(ifr.ifr_name, interface_name, IFNAMSIZ - 1);

  if (ioctl(socket_fd, SIOCGIFNETMASK, &ifr) < 0)
  {
    goto cleanup;
  }

  struct sockaddr_in *netmask = (struct sockaddr_in *)&ifr.ifr_netmask;

  if (inet_ntop(AF_INET, &netmask->sin_addr, mask, INET_ADDRSTRLEN) == NULL)
  {
    goto cleanup;
  }

  status = NM_OK;

cleanup:
  if (socket_fd >= 0)
  {
    close(socket_fd);
  }

  return status;
}

static nm_status_t get_network_gateway(const char *interface_name,
                                       char *gateway)
{
  nm_status_t status = NM_ERROR;
  FILE *file = NULL;
  char line[512];

  if (gateway == NULL || interface_name == NULL)
  {
    goto cleanup;
  }

  file = fopen(GATEWAY_PATH, "r");

  if (file == NULL)
  {
    goto cleanup;
  }

  if (fgets(line, sizeof(line), file) == NULL)
  {
    goto cleanup;
  }

  while (fgets(line, sizeof(line), file))
  {
    char iface[32] = {0};

    char gateway_hex[9];
    char destination_hex[9];
    if (sscanf(line, "%31s %8s %8s", iface, destination_hex, gateway_hex) == 3)
    {
      if (strcmp(interface_name, iface) == 0 &&
          strcmp(destination_hex, "00000000") == 0)
      {
        uint32_t gateway_value = (uint32_t)strtoul(gateway_hex, NULL, 16);
        struct in_addr gateway_addr;
        gateway_addr.s_addr = gateway_value;

        if (inet_ntop(AF_INET, &gateway_addr, gateway, INET_ADDRSTRLEN) == NULL)
        {
          goto cleanup;
        }

        status = NM_OK;

        goto cleanup;
      }
    }
  }

cleanup:
  if (file != NULL)
  {
    fclose(file);
  }
  return status;
}

static nm_status_t get_network_mac_address(const char *interface_name,
                                           char *mac_address)
{
  nm_status_t status = NM_ERROR;
  int socket_fd = -1;
  struct ifreq ifr;

  if (interface_name == NULL || mac_address == NULL)
  {
    goto cleanup;
  }

  ifr = (struct ifreq){0};

  socket_fd = socket(AF_INET, SOCK_DGRAM, 0);

  if (socket_fd < 0)
  {
    goto cleanup;
  }

  strncpy(ifr.ifr_name, interface_name, IFNAMSIZ - 1);

  if (ioctl(socket_fd, SIOCGIFHWADDR, &ifr) < 0)
  {
    goto cleanup;
  }

  unsigned char *mac = (unsigned char *)ifr.ifr_hwaddr.sa_data;

  snprintf(mac_address, 18, "%02X:%02X:%02X:%02X:%02X:%02X", mac[0], mac[1],
           mac[2], mac[3], mac[4], mac[5]);

  status = NM_OK;

cleanup:
  if (socket_fd >= 0)
  {
    close(socket_fd);
  }
  return status;
}

static nm_status_t get_network_mtu(const char *interface_name, int *mtu)
{

  return NM_OK;
}

nm_status_t get_network_info(const char *interface_name,
                             nm_network_info_t *info)
{
  nm_status_t status = NM_ERROR;

  if (info == NULL)
  {
    goto cleanup;
  }

  *info = (nm_network_info_t){0};

  if (get_network_ip_address(interface_name, info->ip_address) != NM_OK)
  {
    goto cleanup;
  }

  if (get_network_mask(interface_name, info->netmask) != NM_OK)
  {
    goto cleanup;
  }

  if (get_network_gateway(interface_name, info->gateway) != NM_OK)
  {
    goto cleanup;
  }

  if (get_network_mac_address(interface_name, info->mac_address) != NM_OK)
  {
    goto cleanup;
  }

  status = NM_OK;
cleanup:
  return status;
}