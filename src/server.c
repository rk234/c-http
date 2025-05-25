#include "server.h"
#include <arpa/inet.h>
#include <stdio.h>
#include <sys/socket.h>

int server_create_socket(in_addr_t ip, uint16_t port) {
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);

  if (sockfd == -1) {
    perror("failed to create socket");
    return -1;
  }

  printf("Socket created successfully!\n");

  struct sockaddr_in addr = {.sin_family = AF_INET,
                             .sin_port = htons(port),
                             .sin_addr.s_addr = htonl(ip)};

  if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) != 0) {
    perror("failed to bind");
    return -1;
  }

  printf("Created and bound socket on port %d\n", port);

  return sockfd;
}

int server_listen_socket(int sockfd) {
  if (listen(sockfd, SOMAXCONN) != 0) {
    perror("failed to listen on socket");
    return -1;
  }
  printf("Server listening for connections!\n");
  return 0;
}

int server_accept_conn(int sockfd, struct sockaddr *client_addr,
                       socklen_t *client_addr_size) {
  int fd = -1;
  if ((fd = accept(sockfd, client_addr, client_addr_size)) == -1) {
    perror("failed to accept connection");
    return -1;
  }

  return fd;
}
