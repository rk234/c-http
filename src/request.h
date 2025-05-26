#ifndef REQUEST_H
#define REQUEST_H

typedef struct {
  char *method;
  char *uri;
  char *host;
  char *user_agent;
} http_req_t;

int parse_http_request(const char *msg, http_req_t *req);

#endif // !REQUEST_H
