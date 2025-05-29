#include "conn_handler.h"
#include "request.h"
#include "response.h"
#include "server.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define RECV_BUF_SIZE 4096

int handle_conn(http_server_t *server, int client_socket_fd,
                struct sockaddr_in *client_addr) {
  printf("[handler]: Handling connection from %s:%u\n",
         inet_ntoa(client_addr->sin_addr), ntohs(client_addr->sin_port));

  int bytes_read = 0;
  char buf[RECV_BUF_SIZE] = {0};

  if ((bytes_read = recv(client_socket_fd, buf, RECV_BUF_SIZE, 0)) != 0) {
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

    if (req.content_type != NULL) {
      printf("\t[handler]: content-type: %s\n", req.content_type);
    }
    if (req.content_type != NULL) {
      printf("\t[handler]: content-length: %d\n", req.content_length);
    }
    if (req.body != NULL) {
      printf("\t[handler]: body:%s\n", req.body);
    }

    int resp_size = 0;

    char *resp = NULL;

    route_handler_fn handler = server_get_handler(server, req.method, req.uri);
    if (handler != NULL) {
      printf("[handler]: found handler for uri (%s)!\n", req.uri);
      http_resp_t res = {0};
      if (handler(&req, &res) != 0) {
        resp = create_http_response_msg(
            (http_resp_t){.body = "<h1>Internal Server Error</h1>",
                          .content_type = "text/html",
                          .max_age = 3600,
                          .status = 500},
            &resp_size);
      } else {
        resp = create_http_response_msg(res, &resp_size);
      }

    } else {
      printf("[handler]: unhandled uri (%s), sending 404!\n", req.uri);
      resp =
          create_http_response_msg((http_resp_t){.body = "<h1>Not Found</h1>",
                                                 .content_type = "text/html",
                                                 .max_age = 3600,
                                                 .status = 404},
                                   &resp_size);
    }

    if (resp != NULL) {
      printf("[handler]: sending the following response:\n%s\n", resp);
      send(client_socket_fd, resp, resp_size, 0);
      free(resp);
    } else {
      printf("[handler]: null response!\n");
    }

    free_http_request(&req);

    memset(buf, 0, RECV_BUF_SIZE);
  }

  return 0;
}
