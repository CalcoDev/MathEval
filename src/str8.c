#include "str8.h"

#include <stdlib.h>
#include <errno.h>

b8 char_is_number(char c)
{
  return c == '0' || c == '1' || c == '2' || c == '3' || c == '4' || c == '5' 
    || c == '6' || c == '7' || c == '8' || c == '9';
}

b8 str8_starts_with(str8 s, str8 cmp)
{
  if (cmp.length > s.length)
    return 0;

  for (i32 i = 0; i < cmp.length; ++i)
  {
    if (s.buffer[i] != cmp.buffer[i])
      return 0;
  }

  return 1;
}

b8 str8_equals(str8 s, str8 cmp)
{
  if (cmp.length != s.length)
    return 0;

  for (i32 i = 0; i < s.length; ++i)
  {
    if (s.buffer[i] != cmp.buffer[i])
      return 0;
  }

  return 1;
}

b8 str8_to_number(str8 s, f32* value)
{
  char* end;
  f32 v = strtod(s.buffer, &end);
  if (value)
    *value = v;

  // TODO(calco): Do this lol
  // if (errno == ERANGE)
  //   return 0;

  return 1;
}