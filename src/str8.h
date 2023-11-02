#ifndef STR8_H
#define STR8_H

#include "defines.h"

#include <string.h>

typedef struct str8
{
  const char* buffer;
  i32 length;
} str8;

#define str8_fmt "%.*s"
#define str8_arg(s) (s).length, (s).buffer

#define str8_empty() (str8) {NULL, 0};

#define str8_make(s) (str8) {s, strlen(s)}
#define str8_make_buffer(buffer) (str8) {buffer, sizeof(buffer)}

b8 char_is_number(char c);

b8 str8_starts_with(str8 s, str8 cmp);
b8 str8_equals(str8 s, str8 cmp);

b8 str8_to_number(str8 s, f32* value);

#endif