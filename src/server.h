#ifndef SERVER_H
#define SERVER_H
#include <netinet/in.h>
#include <stdint.h>
int server_create_socket(in_addr_t ip, uint16_t socket);
int server_listen_socket(int sockfd);
int server_accept_conn(int sockfd, struct sockaddr *client_addr,
                       socklen_t *client_addr_size);
#endif // !SERVER_H
