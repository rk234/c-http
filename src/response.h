#ifndef RESPONSE_H
#define RESPONSE_H

typedef struct {
  int status;
  char *body;
} http_resp_t;

void create_http_response();

#endif // !RESPONSE_H
