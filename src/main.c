#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

typedef uint8_t i8;
typedef uint16_t i16;
typedef uint32_t i32;
typedef uint64_t i64;

typedef float f32;
typedef double f64;

typedef i8 b8;
typedef i16 b16;
typedef i32 b32;
typedef i64 b64;

#define me_assert(e, code, ...) if (!(e)) { \
  fprintf(stderr, __VA_ARGS__); \
  exit(code); \
}
#define me_log(format, ...) { \
  fprintf(stderr, "%s:%d - ", __FILE__, __LINE__); \
  fprintf(stderr, format, __VA_ARGS__); \
  fprintf(stderr, "\n"); \
}
#define me_err(format, ...) { \
  fprintf(stderr, "%s:%d - ", __FILE__, __LINE__); \
  fprintf(stderr, format, __VA_ARGS__); \
  fprintf(stderr, "\n"); \
}
#define me_output(format, ...) { \
  fprintf(stdout, format, __VA_ARGS__); \
  fprintf(stdout, "\n"); \
}

typedef struct str8
{
  char* str;
  i32 length;
} str8;

#define str8_make(s) (str8) {s, strlen(s)}

b8 str8_starts_with(str8 s, str8 cmp)
{
  if (cmp.length > s.length)
    return 0;

  for (i32 i = 0; i < s.length; ++i)
  {
    if (s.str[i] != cmp.str[i])
      return 0;
  }

  return 1;
}

typedef struct str8_view
{
  char* str;
  i32 start;
  i32 end;
} str8_view;

typedef enum error_codes
{
  ERROR_CODE_NOT_ENOUGH_ARGUMENTS,
  ERROR_CODE_INVALID_EXPRESSION,
  ERROR_CODE_INVALID_FILE_PATH,
} error_codes;

typedef struct parse_res
{
  f32 value;
  i32 error;
} parse_res;

#define parse_res_make_error(code) (parse_res) {-1.f, code}
#define parse_res_make_success(value) (parse_res) {value, 0}

parse_res parse_expr(str8 expr)
{
  return parse_res_make_success(420);
}

int main(int argc, char** argv)
{
  me_assert(argc > 1,
    ERROR_CODE_NOT_ENOUGH_ARGUMENTS, 
    "Received too few arguments!\nAppend --h for help!"
  );
  
  str8 arg1 = str8_make(argv[1]);
  if (str8_starts_with(arg1, str8_make("--f")))
  {
    me_assert(argc > 2, 
      ERROR_CODE_NOT_ENOUGH_ARGUMENTS,
      "Please specify the file after the --f flag!"
    );

    arg1 = str8_make(argv[2]);
    me_log("Was given file input: %s", arg1.str);
  }

  parse_res result = parse_expr(arg1);
  me_assert(result.error != 0, 
    result.error,
    "Failed to parse expression."
  );

  me_output("%.8f", result.value);
  return 0;
}