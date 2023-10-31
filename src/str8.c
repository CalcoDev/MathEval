#include "str8.h"

b8 str8_starts_with(str8 s, str8 cmp)
{
  if (cmp.length > s.length)
    return 0;

  for (i32 i = 0; i < s.length; ++i)
  {
    if (s.buffer[i] != cmp.buffer[i])
      return 0;
  }

  return 1;
}