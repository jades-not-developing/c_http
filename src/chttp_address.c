#include "chttp_address.h"
#include "chttp_string.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

CHttpAddress chttp_parse_address(int len, const char* url)
{
  CHttpAddress addr = {0};
  str_init(&addr.hostname);
  str_init(&addr.port);

  int initial_index = -1;

  {
    char* proto_buf = malloc(sizeof(char) * 8);
    strncpy(proto_buf, url, 8);
    proto_buf[7] = 0;

    if (strcmp(proto_buf, "http://") == 0) {
      addr.ssl = 0;
      initial_index = 7;
    }
    free(proto_buf);
  }

  if (initial_index == -1) {
    char* proto_buf = malloc(sizeof(char) * 9);
    strncpy(proto_buf, url, 8);
    proto_buf[8] = 0;

    if (strcmp(proto_buf, "https://") == 0) {
      addr.ssl = 1;
      initial_index = 8;
    }
    free(proto_buf);
  }

  if (initial_index == -1) {
    // TODO(jade): Proper error reporting via error values
    assert(0 && "Failed to find protocol prefix");
  }

  for (int i = initial_index; i < len; ++i)
  {
    char c = url[i];
    if (c == 0 || c == '/') {
      // TODO(jade): implement port parsing
      if (addr.port.count == 0) {
        addr.port.str = "80";
        addr.port.count = 2;
        addr.port.capacity = 3;
      }
      break;  
    }
    str_push(&addr.hostname, c);
  }

  return addr;
}
