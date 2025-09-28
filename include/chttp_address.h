#pragma once

#include "chttp_string.h"

typedef struct CHttpAddress {
  String hostname;
  String port;

  _Bool ssl;
} CHttpAddress;

CHttpAddress chttp_parse_address(int len, const char* url);
