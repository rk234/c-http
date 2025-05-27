#ifndef RESPONSE_H
#define RESPONSE_H

typedef struct {
  int status;
  char *content_type;
  char *body;
  int max_age;
} http_resp_t;

char *create_http_response_msg(http_resp_t res, int *buf_size);

#endif // !RESPONSE_H
