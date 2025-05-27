#include "server.h"
#include "conn_handler.h"
#include <arpa/inet.h>
#include <asm-generic/socket.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>

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

  int yes = 1;
  if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) != 0) {
    perror("failed to set SO_REUSEADDR");
  }

  if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) != 0) {
    close(sockfd);
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
