#include "network_info.h"

#include <arpa/inet.h>
#include <net/if.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <unistd.h>

nm_status_t get_network_info(const char *interface_name,
                             nm_network_info_t *info)
{
  nm_status_t status = NM_ERROR;

  int socket_fd = -1;
  struct ifreq ifr;

  if (interface_name == NULL || info == NULL)
  {
    goto cleanup;
  }

  *info = (nm_network_info_t){0};
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

  if (inet_ntop(AF_INET, &address->sin_addr, info->ip_address,
                sizeof(info->ip_address)) == NULL)
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
