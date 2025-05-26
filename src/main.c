#include "conn_handler.h"
#include "server.h"
#include <bits/getopt_core.h>
#include <bits/pthreadtypes.h>
#include <netinet/in.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>

int should_exit = 0;
int socket_fd = -1;

void cleanup() { close(socket_fd); }

void exit_handler(int signo) {
  printf("exiting gracefully...\n");
  should_exit = 1;
  cleanup();
  exit(0);
}

void child_handler(int signo) {
  int pid;
  if ((pid = wait(NULL)) != -1) {
    printf("[child_hanlder]: child (%d) exited!\n", pid);
  }
}

int main(int argc, char *argv[]) {
  printf("pid: %d\n", getpid());

  if (signal(SIGTERM, exit_handler) == SIG_ERR) {
    printf("failed to set SIGTERM handler!\n");
  }

  if (signal(SIGINT, exit_handler) == SIG_ERR) {
    printf("failed to set SIGINT handler!\n");
  }

  if (signal(SIGCHLD, child_handler) == SIG_ERR) {
    printf("failed to set SIGCHLD handler!\n");
  }

  if ((socket_fd = server_create_socket(INADDR_ANY, 8080)) == -1) {
    printf("Exiting...\n");

    return -1;
  }

  server_listen_socket(socket_fd);

  while (!should_exit) {
    struct sockaddr client_addr = {0};
    socklen_t client_addr_len = 0;

    int client_socket_fd = -1;
    if ((client_socket_fd = server_accept_conn(socket_fd, &client_addr,
                                               &client_addr_len)) != -1) {
      printf("Connection accepted!\n");

      int pid = fork();

      if (pid == 0) {
        // child
        close(socket_fd);

        if (handle_conn(client_socket_fd, (struct sockaddr_in *)&client_addr) !=
            0) {
          close(client_socket_fd);
          printf("[handler]: exited with error!\n");
          return -1;
        }
        close(client_socket_fd);
        printf("[handler]: exited successfully\n");
        return 0;
      }
      close(client_socket_fd);
    }
  }

  cleanup();
  return 0;
}
