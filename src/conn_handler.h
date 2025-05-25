#ifndef CONN_HANDLER_H
#define CONN_HANDLER_H
#include <netinet/in.h>

int handle_conn(int client_socket_fd, struct sockaddr_in *client_addr);

#endif // !CONN_HANDLER_H
