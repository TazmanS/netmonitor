#ifndef NETWORK_INFO_H
#define NETWORK_INFO_H

#include "common.h"

typedef struct
{
  char ip_address[16];
} nm_network_info_t;

nm_status_t get_network_info(const char *interface_name,
                             nm_network_info_t *info);

#endif