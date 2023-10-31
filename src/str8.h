#ifndef STR8_H
#define STR8_H

#include "defines.h"

typedef struct str8
{
  char* buffer;
  i32 length;
} str8;

#define str8_make(s) (str8) {s, strlen(s)}
#define str8_make_buffer(buffer) (str8) {buffer, sizeof(buffer)}

b8 str8_starts_with(str8 s, str8 cmp);

#endif