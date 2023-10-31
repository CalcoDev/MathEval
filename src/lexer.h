#ifndef LEXER_H
#define LEXER_H

#include "defines.h"
#include "str8.h"

typedef enum lex_token_type_t
{
  TOKEN_TYPE_EOF,
  TOKEN_TYPE_NUMBER,
  TOKEN_TYPE_PLUS,
  TOKEN_TYPE_MINUS,
  TOKEN_TYPE_MULT,
  TOKEN_TYPE_DIV,
  TOKEN_TYPE_EXP,
  TOKEN_TYPE_SQRT,
  TOKEN_TYPE_LOG,
  TOKEN_TYPE_OPEN_PAREN,
  TOKEN_TYPE_CLOSE_PAREN,
  TOKEN_TYPE_UNKNOWN_TOKEN
} lex_token_type_t;

typedef struct lex_token_view
{
  i32 start;
  i32 end;
} lex_token_view_t;

typedef struct lex_token
{
  lex_token_type_t token_type;
  lex_token_view_t lexeme;
} lex_token_t;

typedef struct lexer
{
  char* buffer;
  i32 start;
  i32 curr;
  i32 end;
} lexer_t;

lexer_t lexer_make(str8 buffer);
lex_token_t lexer_get_next_token(lexer_t* lexer);

#endif