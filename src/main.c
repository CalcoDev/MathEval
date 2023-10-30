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
  ERROR_CODE_NO_ERROR,
  ERROR_CODE_NOT_ENOUGH_ARGUMENTS,
  ERROR_CODE_INVALID_ARGUMENT,
  ERROR_CODE_INVALID_EXPRESSION,
  ERROR_CODE_INVALID_FILE_PATH,
} error_codes;

typedef struct parse_res
{
  f32 value;
  error_codes error;
} parse_res;

#define parse_res_make_error(code) (parse_res) {-1.f, code}
#define parse_res_make_success(value) (parse_res) {value, ERROR_CODE_NO_ERROR}

parse_res parse_expr(str8 expr)
{
  return parse_res_make_success(420);
}

str8 os_parse_file(str8 filepath)
{
  return filepath;
}

int main(int argc, char** argv)
{
  if (argc == 1)
    goto help;

  str8 s = str8_make(argv[1]);
  if (str8_starts_with(s, str8_make("--h")))
    goto help;

  b8 bexpr = str8_starts_with(s, str8_make("--e"));
  b8 bfile = str8_starts_with(s, str8_make("--f"));

  if (!bexpr && !bfile)
    me_assert(0, 
      ERROR_CODE_INVALID_ARGUMENT,
      "Received unknown argument!\nAppend --h for a help command!"
    );
  
  me_assert(argc > 2, 
    ERROR_CODE_NOT_ENOUGH_ARGUMENTS,
    "Missing arguments!\nAppend --h for a help command!"
  );
  
  s = bexpr ? str8_make(argv[2]) : os_parse_file(str8_make(argv[2]));
  parse_res result = parse_expr(s);
  me_assert(result.error == 0, 
    result.error,
    "Failed to parse expression."
  );
  me_output("%.8f", result.value);
  return 0;

help:
  me_output(
    "-- Calcopod's Math Eval --\n"
    "A random math expression evaluator.\n"
    "\n\n"
    "Example usage:\n"
    "/path/to/math_eval --e \"0.5 + log(12, 1 / ( sqrt(9^3) ) )\"\n"
    "\n\n"
    "Arguments:\n"
    "--h - Prints this message!\n"
    "--e [expression] - Specifies the expression to evaluate.\n"
    "--f [file] - Specifies the file to read and evaluate.\n"
    "\n\n"
    "Happy use!"
  );
  return 0;
}