#pragma once

typedef struct CHttpResponse {
    _Bool valid;

    const char* method;
    const char* url;
    const char* error;

    const char* result;
} CHttpResponse;

CHttpResponse chttp_get(const char* url);