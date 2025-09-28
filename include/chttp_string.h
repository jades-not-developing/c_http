#pragma once

typedef struct String {
  size_t count;
  size_t capacity;
  char* str;
} String;

void str_init(String* s);
void str_push(String* s, char c);
void str_append(String* s, size_t len, const char* other);
