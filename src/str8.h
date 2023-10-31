#ifndef STR8_H
#define STR8_H

#include "defines.h"

#include <string.h>

typedef struct str8
{
  char* buffer;
  i32 length;
} str8;

typedef struct str8_view
{
  char* buffer;
  i32 start;
  i32 end;
} str8_view;

#define str8_make(s) (str8) {s, strlen(s)}
#define str8_make_buffer(buffer) (str8) {buffer, sizeof(buffer)}

b8 char_is_number(char c);

b8 str8_starts_with(str8 s, str8 cmp);
b8 str8_equals(str8 s, str8 cmp);

#endif