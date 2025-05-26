#include "conn_handler.h"
#include "request.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define RECV_BUF_SIZE 4096

int handle_conn(int client_socket_fd, struct sockaddr_in *client_addr) {
  printf("[handler]: Handling connection from %s:%u\n",
         inet_ntoa(client_addr->sin_addr), ntohs(client_addr->sin_port));

  int bytes_read = 0;
  char buf[RECV_BUF_SIZE] = {0};
  while ((bytes_read = recv(client_socket_fd, buf, RECV_BUF_SIZE, 0)) != 0) {
    if (bytes_read == -1) {
      perror("[handler]: read from socket failed");
      return -1;
    }

    printf("read: %d bytes\n", bytes_read);

    http_req_t req = {0};

    if (parse_http_request(buf, &req)) {
      printf("failed to parse HTTP request!\n");
      return -1;
    }

    printf("\t[handler]: Receive request\n");
    printf("\t[handler]: method: %s\n", req.method);
    printf("\t[handler]: uri: %s\n", req.uri);
    printf("\t[handler]: host: %s\n", req.host);

    if (req.body != NULL) {
      printf("\t[handler]: body:\n%s\n", req.body);
    }

    free_http_request(&req);

    memset(buf, 0, RECV_BUF_SIZE);
  }

  return 0;
}
