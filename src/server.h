#ifndef SERVER_H
#define SERVER_H
#include "request.h"
#include "response.h"
#include <netinet/in.h>
#include <stdint.h>

typedef int (*route_handler_fn)(http_req_t *req, http_resp_t *resp);

typedef struct route_handler_list {
  route_handler_fn handler;
  char *route;
  const char *method;
  struct route_handler_list *next;
} route_handler_node_t;

typedef struct {
  int sock_fd;
  route_handler_node_t *route_handlers;
} http_server_t;

int server_create(http_server_t *server, in_addr_t ip, uint16_t socket);
int server_listen_socket(http_server_t *server);
int server_accept_conn(http_server_t *server, struct sockaddr *client_addr,
                       socklen_t *client_addr_size);

int server_add_handler(http_server_t *server, const char *method,
                       const char *route, route_handler_fn handler);
route_handler_fn server_get_handler(http_server_t *server, const char *method,
                                    const char *route);

void server_cleanup(http_server_t *server);
#endif // !SERVER_H
