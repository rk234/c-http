#include "conn_handler.h"
#include <stdio.h>

int handle_conn(int client_socket_fd) {
  printf("Hello from connection handler\n");
  return 0;
}
