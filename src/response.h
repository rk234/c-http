#ifndef RESPONSE_H
#define RESPONSE_H

typedef struct {
  int status;
  char *content_type;
  char *body;
} http_resp_t;

int create_http_response(http_resp_t resp, char *buf, int buf_size);

#endif // !RESPONSE_H
