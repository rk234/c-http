#include "conn_handler.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define RECV_BUF_SIZE 4096

int handle_conn(int client_socket_fd, struct sockaddr_in *client_addr) {
  printf("[handler]: Handling connection from %s:%u\n",
         inet_ntoa(client_addr->sin_addr), ntohs(client_addr->sin_port));

  int bytes_read = 0;
  uint8_t buf[RECV_BUF_SIZE] = {0};
  while ((bytes_read = read(client_socket_fd, buf, RECV_BUF_SIZE)) != 0) {
    if (bytes_read == -1) {
      perror("[handler]: read from socket failed");
      return -1;
    }

    memset(buf, 0, RECV_BUF_SIZE);
  }

  printf("[handler]: read: %s", buf);

  return 0;
}
