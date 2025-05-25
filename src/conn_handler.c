#include "conn_handler.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

#define READ_BUF_SIZE 1024

int handle_conn(int client_socket_fd, struct sockaddr_in *client_addr) {
  printf("[handler]: Handling connection from %s:%u\n",
         inet_ntoa(client_addr->sin_addr), ntohs(client_addr->sin_port));

  uint8_t buf[READ_BUF_SIZE] = {0};

  if (read(client_socket_fd, buf, READ_BUF_SIZE) == -1) {
    perror("[hanler]: read from socket failed");
    return -1;
  }

  printf("[handler]: read: %s", buf);

  return 0;
}
