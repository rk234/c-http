#include "request.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *tok(char **str, char *delim) {
  if (delim == NULL)
    return *str;

  char *res = *str;

  for (int i = 0; i < strlen(delim); i++) {
    char c = delim[i];
    if (i == 0)
      res = strsep(str, &c);
    else
      strsep(str, &c);
  }

  return res;
}

int parse_http_request(char *msg, http_req_t *req) {
  printf("[req]: received msg:\n%s\n", msg);

  char *line = tok(&msg, "\r\n");
  int i = 0;

  while (line != NULL) {
    // printf("[req]: line: %s\n", line);
    if (i == 0) { // start line
      char *method = tok(&line, " ");
      char *target = tok(&line, " ");
      char *proto = tok(&line, " ");

      if (method == NULL || target == NULL || proto == NULL) {
        printf("[req]: invalid request start line!");
        return -1;
      }

      printf("[req]: parsed start line: method: %s; target: %s; proto: %s\n",
             method, target, proto);

      if (strcmp(proto, "HTTP/1.1") != 0) {
        printf("[req]: invalid protocol: %s!\n", proto);
        return -1;
      }

      req->method = strdup(method);
      req->uri = strdup(target);
    } else if (strlen(line) == 0) { // empty line, what follows is body
      printf("[req]: found body\n");
      char *body = tok(&msg, "\0");

      if (body != NULL)
        req->body = strdup(body);

      break;
    } else {
      char *header_name = tok(&line, ":");
      char *value = tok(&line, "\r\n");

      if (header_name == NULL || value == NULL) {
        printf("[req]: invalid header format!\n");
        return -1;
      }

      // remove leading whitespace
      while (*value == ' ')
        value++;

      printf("[req]: parse header: (%s: %s)\n", header_name, value);

      if (strcmp(header_name, "Host") == 0) {
        req->host = strdup(value);
      } else if (strcmp(header_name, "User-Agent") == 0) {
        req->user_agent = strdup(value);
      } else if (strcmp(header_name, "Content-Type") == 0) {
        req->content_type = strdup(value);
      }
    }

    // printf("[req]: remaining msg:\n%s\n", msg);
    line = tok(&msg, "\r\n");
    i++;
  }

  return 0;
}

void free_http_request(http_req_t *req) {
  free(req->host);
  free(req->method);
  free(req->uri);
  free(req->user_agent);
  free(req->content_type);
  free(req->body);

  req->host = NULL;
  req->method = NULL;
  req->uri = NULL;
  req->user_agent = NULL;
  req->content_type = NULL;
  req->body = NULL;
}
