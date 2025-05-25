#include "conn_handler.h"
#include "server.h"
#include <bits/getopt_core.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  int socket_fd = -1;
  if ((socket_fd = server_create_socket(INADDR_ANY, 8080)) == -1) {
    printf("Exiting...\n");
    return -1;
  }

  server_listen_socket(socket_fd);

  while (1) {
    struct sockaddr client_addr = {0};
    socklen_t client_addr_len = 0;

    int client_socket_fd = -1;
    if ((client_socket_fd = server_accept_conn(socket_fd, &client_addr,
                                               &client_addr_len)) != -1) {
      printf("Connection accepted!\n");

      int pid = fork();

      if (pid == 0) {
        // child
        if (handle_conn(client_socket_fd, (struct sockaddr_in *)&client_addr) !=
            0) {
          printf("[handler]: exited with error!\n");
          return 1;
        }
        printf("[handler]: exited successfully\n");
        return 0;
      }
      close(client_socket_fd);
    }
  }

  return 0;
}
