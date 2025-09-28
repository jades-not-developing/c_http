#include "chttp_string.h"

#include <stdlib.h>

void str_init(String* s)
{
  s->count = 0;
  s->capacity = 8;
  s->str = malloc(s->capacity * sizeof(char));
  s->str[0] = 0;
}

void str_push(String* s, char c)
{
  if (s->count + 2 > s->capacity) {
    s->capacity += 8;
    s->str = realloc((void*)s->str, s->capacity * sizeof(char));
  }

  s->str[s->count++] = c;
  s->str[s->count] = 0x0;
}

void str_append(String* s, size_t len, const char* other)
{
  for (int i = 0; i < len; i++)
    str_push(s, other[i]);
}
