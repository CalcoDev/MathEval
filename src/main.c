#include <stdlib.h>
#include <string.h>

#include "defines.h"
#include "str8.h"
#include "os_utils.h"

#include "lexer.h"
#include "parser.h"

typedef enum error_codes
{
  ERROR_CODE_NO_ERROR,
  ERROR_CODE_INTERNAL_ERROR,
  ERROR_CODE_NOT_ENOUGH_ARGUMENTS,
  ERROR_CODE_INVALID_ARGUMENT,
  ERROR_CODE_INVALID_EXPRESSION,
  ERROR_CODE_INVALID_FILE_PATH,
  ERROR_CODE_LEXING_UNKNOWN_TOKEN
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
  lex_token_t tokens[256];
  parser_node_t nodes[256];
  
  lexer_t lexer = lexer_make(expr);
  parser_t parser = parser_make(&tokens[0], token_count);

  i32 token_count = 0;
  for (; 1; ++token_count)
  {
    tokens[token_count] = lexer_get_next_token(&lexer);
    if (tokens[token_count].token_type == TOKEN_TYPE_EOF)
      break;
    
    me_assert(tokens[token_count].token_type != TOKEN_TYPE_UNKNOWN_TOKEN,
      ERROR_CODE_LEXING_UNKNOWN_TOKEN,
      "Found unknown token at character %d!", tokens[token_count].lexeme.start
    );
  }
  me_log("Lexing complete!");
  
  parser_parse(&parser, &nodes[0], 256);
  me_log("Parsing complete!");

  return parse_res_make_success(420);
}

void parse_file(str8 filepath, str8* content)
{
  char fullpath_buf[256];
  str8 fullpath = str8_make_buffer(fullpath_buf);
  
  char exe_buf[256];
  str8 exe_path = str8_make_buffer(exe_buf);
  
  os_get_exe_path(&exe_path);
  me_assert(exe_path.length != -1,
    ERROR_CODE_INTERNAL_ERROR,
    "Failed getting executable path...\nContact support ig?"
  );

  os_windowsify_path(&filepath);
  os_rel_path(exe_path, filepath, &fullpath);
  me_assert(fullpath.length != -1,
    ERROR_CODE_INTERNAL_ERROR,
    "Failed getting relative path...\nContact support ig?"
  );
  
  u8 file_exists = os_file_exists(fullpath);
  me_assert(file_exists == 1,
    ERROR_CODE_INVALID_FILE_PATH,
    "Specified filepath is invalid. " 
    "It might not exist or it might be a directory.\n"
    "Please check the filepath then run the command again."
  );

  os_read_file(fullpath, content);
  me_assert(content->length != -1,
    ERROR_CODE_INTERNAL_ERROR,
    "Failed reading contents of file...\nContact support ig?"
  );
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
  
  char filecontent_buf[4096];
  if (bexpr)
    s = str8_make(argv[2]);
  else
  {
    s = str8_make_buffer(filecontent_buf);
    parse_file(str8_make(argv[2]), &s);
  }

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