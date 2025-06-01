#include "response.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static const char *template = "HTTP/1.1 %d %s\r\n"
                              "Server: c-http\r\n"
                              "Date: %s\r\n"
                              "Cache-Control: public, max-age=%d\r\n"
                              "Content-Type: %s\r\n"
                              "\r\n"
                              "%s";

char *create_http_response_msg(http_resp_t res, int *buf_size) {
  char dateBuf[512] = {0};
  time_t now = time(0);
  struct tm tm = *gmtime(&now);
  strftime(dateBuf, sizeof(dateBuf), "%a, %d %b %Y %H:%M:%S %Z", &tm);

  int len = snprintf(NULL, 0, template, res.status, "msg", dateBuf, res.max_age,
                     res.content_type, res.body);

  printf("[response]: response length: %d\n", len);

  char *response = malloc(sizeof(char) * (len + 1));

  if (response == NULL)
    return NULL;

  snprintf(response, len + 1, template, res.status, "msg", dateBuf, res.max_age,
           res.content_type, res.body);
  response[len] = '\0';
  *buf_size = len;

  return response;
}
