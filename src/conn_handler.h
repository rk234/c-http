#ifndef CONN_HANDLER_H
#define CONN_HANDLER_H
#include "request.h"
#include "response.h"
#include "server.h"
#include <netinet/in.h>

int handle_conn(http_server_t *server, int client_socket_fd,
                struct sockaddr_in *client_addr);

#endif // !CONN_HANDLER_H
