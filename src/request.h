#ifndef REQUEST_H
#define REQUEST_H

typedef struct {
  char *method;
  char *uri;
  char *host;
  char *user_agent;
  char *content_type;
  char *body;
} http_req_t;

int parse_http_request(char *msg, http_req_t *req);
void free_http_request(http_req_t *req);

#endif // !REQUEST_H
