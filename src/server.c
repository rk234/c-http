#include "server.h"
#include "conn_handler.h"
#include <arpa/inet.h>
#include <asm-generic/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

int server_create(http_server_t *server, in_addr_t ip, uint16_t port) {
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

  server->sock_fd = sockfd;
  server->route_handlers = NULL;

  return sockfd;
}

int server_listen_socket(http_server_t *server) {
  if (listen(server->sock_fd, SOMAXCONN) != 0) {
    perror("failed to listen on socket");
    return -1;
  }
  printf("Server listening for connections!\n");
  return 0;
}

int server_accept_conn(http_server_t *server, struct sockaddr *client_addr,
                       socklen_t *client_addr_size) {
  int fd = -1;
  if ((fd = accept(server->sock_fd, client_addr, client_addr_size)) == -1) {
    perror("failed to accept connection");
    return -1;
  }

  return fd;
}

int server_add_handler(http_server_t *server, const char *route,
                       route_handler_fn handler) {
  route_handler_node_t *new_node = malloc(sizeof(route_handler_node_t));

  if (new_node == NULL)
    return -1;

  new_node->handler = handler;
  new_node->route = strdup(route);
  new_node->next = NULL;

  if (server->route_handlers == NULL) {
    server->route_handlers = new_node;
    return 0;
  }

  route_handler_node_t *last_node = server->route_handlers;

  while (last_node->next != NULL) {
    last_node = last_node->next;
  }

  last_node->next = new_node;
  return 0;
}

route_handler_fn server_get_handler(http_server_t *server, const char *route) {
  if (server->route_handlers == NULL)
    return NULL;

  for (route_handler_node_t *node = server->route_handlers; node != NULL;
       node = node->next) {
    if (strcmp(node->route, route) == 0)
      return node->handler;
  }

  return NULL;
}

void server_cleanup(http_server_t *server) {
  close(server->sock_fd);
  route_handler_node_t *node = server->route_handlers;
  while (node != NULL) {
    route_handler_node_t *next = node->next;
    free(node->route);
    free(node);
    node = next;
  }
}
