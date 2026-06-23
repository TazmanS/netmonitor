#ifndef TCP_MONITOR_HELPERS_H
#define TCP_MONITOR_HELPERS_H

#include "common.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>

const char *tcp_state_to_string(const char *state);

nm_status_t split_tcp_address(
    const char *address,
    char *ip_hex,
    size_t ip_hex_size,
    char *port_hex,
    size_t port_hex_size);

nm_status_t hex_to_port(
    const char *port_hex,
    unsigned int *port);

nm_status_t hex_to_ipv4(
    const char *hex_ip,
    char *ip,
    size_t ip_size);

nm_status_t parse_tcp_address(
    const char *address,
    char *ip,
    size_t ip_size,
    unsigned int *port);

#endif