#include "conn_handler.h"
#include "request.h"
#include "response.h"
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
http_server_t server = {0};

void cleanup() { server_cleanup(&server); }

void exit_handler(int signo) {
  printf("exiting gracefully...\n");
  should_exit = 1;
  cleanup();
  exit(0);
}

void child_handler(int signo) {
  int pid;
  int status;
  if ((pid = wait(&status)) != -1) {
    printf("[child_hanlder]: child (%d) exited! status: %d\n", pid,
           WEXITSTATUS(status));
  }
}

int hello_handler(http_req_t *req, http_resp_t *resp) {
  resp->body = ("<p>This request was handled by the hello handler!</p>");
  resp->content_type = "text/html";
  resp->status = 200;
  resp->max_age = 3600;
  return 0;
}

int foo_handler(http_req_t *req, http_resp_t *resp) {
  resp->body = ("<p>This request was handled by the foo handler!</p>");
  resp->content_type = "text/html";
  resp->status = 200;
  resp->max_age = 3600;
  return 0;
}

int index_handler(http_req_t *req, http_resp_t *resp) {
  resp->body = "<h1>Hello World!</h1>\n"
               "<p>This page is being served by c-http!</p>\n"
               "<a href='/hello'>Go to another page</a>\n"
               "<a href='/foo'>Or another one</a>\n";
  resp->content_type = "text/html";
  resp->status = 200;
  resp->max_age = 3600;
  return 0;
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

  if (server_create(&server, INADDR_ANY, 8080) == -1) {
    printf("exiting...\n");

    return -1;
  }

  server_add_handler(&server, "/", index_handler);
  server_add_handler(&server, "/hello", hello_handler);
  server_add_handler(&server, "/foo", foo_handler);

  server_listen_socket(&server);

  while (!should_exit) {
    struct sockaddr client_addr = {0};
    socklen_t client_addr_len = 0;

    int client_socket_fd = -1;
    if ((client_socket_fd = server_accept_conn(&server, &client_addr,
                                               &client_addr_len)) != -1) {
      printf("Connection accepted!\n");

      int pid = fork();

      if (pid == 0) {
        // child
        close(server.sock_fd);

        if (handle_conn(&server, client_socket_fd,
                        (struct sockaddr_in *)&client_addr) != 0) {
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
