#define _CRT_SECURE_NO_DEPRECATE
#include <stdlib.h>
#include <string.h>

#include "defines.h"
#include "str8.h"

#include "lexer.h"
#include "parser.h"

typedef enum
{
  ERROR_CODE_NO_ERROR,
  ERROR_CODE_INTERNAL_ERROR,
  ERROR_CODE_NOT_ENOUGH_ARGUMENTS,
  ERROR_CODE_INVALID_ARGUMENT,
  ERROR_CODE_INVALID_EXPRESSION,
  ERROR_CODE_INVALID_FILE_PATH,
  ERROR_CODE_LEXING_UNKNOWN_TOKEN
} error_codes_t;

typedef struct
{
  f32 value;
  error_codes_t error;
} result_t;

#define res_make_error(code) (result_t) {-1.f, code}
#define res_make_success(value) (result_t) {value, ERROR_CODE_NO_ERROR}

str8 slurp_file(char* path)
{
  i32 slen = 0;
  char* sbuf = NULL;

  FILE* f = fopen(path, "rb");
  if (f == NULL)
    goto error;
  
  if (fseek(f, 0, SEEK_END) < 0)
    goto error;
  
  slen = ftell(f);
  if (slen < 0)
    goto error;
  
  sbuf = malloc(slen);
  if (sbuf == NULL)
    goto error;
  
  if (fseek(f, 0, SEEK_SET) < 0)
    goto error;
  
  u64 n = fread(sbuf, 1, slen, f);
  if (n != (u64) slen)
    goto error;
  
  if (ferror(f))
    goto error;
  
  fclose(f);
  return (str8) { sbuf, n };

error:
  if (f)
    fclose(f);
  
  if (sbuf)
    free(sbuf);
  
  return str8_empty();
}

result_t parse_expr(str8* expr)
{
  lexer_t lexer = (lexer_t) { expr->buffer, 0, 0, expr->length };
  
  i32 token_count;
  me_assert(lexer_estimate_token_count(&lexer, &token_count) != -1,
    ERROR_CODE_LEXING_UNKNOWN_TOKEN,
    "Tokenization failed!\nFound unknown token at character %d", token_count
  );

  lex_token_t* tokens = (lex_token_t*)
    (malloc(token_count * sizeof(lex_token_t)));
  me_assert(tokens != NULL, 
    ERROR_CODE_INTERNAL_ERROR, 
    "Error allocating token memory!"
  );

  parser_node_t* nodes = (parser_node_t*)
    (malloc(token_count * sizeof(parser_node_t)));
  me_assert(nodes != NULL, 
    ERROR_CODE_INTERNAL_ERROR, 
    "Error allocating node memory!"
  );
  
  lexer_tokenize(&lexer, tokens);
  
  parser_t parser = (parser_t) { tokens, token_count };
  parser_node_t* head = parser_parse(&parser, nodes, NULL);

  // TODO(calco): Evaluate AST

  free(tokens);
  free(nodes);

  return res_make_success(420);
}

int main(int argc, char** argv)
{
  if (argc < 2)
    goto help;

  str8 s = str8_make(argv[1]);
  if (str8_starts_with(s, str8_make("--h")))
    goto help;

  b8 bexpr = str8_starts_with(s, str8_make("--e"));
  b8 bfile = str8_starts_with(s, str8_make("--f"));

  if (!bexpr && !bfile)
  {
    me_assert(0, 
      ERROR_CODE_INVALID_ARGUMENT,
      "Received unknown argument!\nAppend --h for a help command!"
    );
  }
  
  s = bexpr ? str8_make(argv[2]) : slurp_file(argv[2]);
  me_assert(s.length != 0, 
    ERROR_CODE_INVALID_EXPRESSION, 
    "Provided expression is empty! Please provide a valid expression!\n"
    "(if the --f flag was provided, please check the filepath and try again!)"
  );

  result_t result = parse_expr(&s);

  // Should always be true.
  if (s.buffer)
    free((char*)s.buffer);

  me_assert(result.error == 0, 
    result.error,
    "Failed to parse expression."
  );
  me_output("%.8f", result.value);

  return ERROR_CODE_NO_ERROR;

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
  return ERROR_CODE_NO_ERROR;
}