#ifndef LEXER_H
#define LEXER_H

#include "defines.h"
#include "str8.h"

typedef struct lex_token_view_s lex_token_view_t;
typedef struct lex_token_s lex_token_t;
typedef struct lexer_s lexer_t;

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

struct lex_token_view_s
{
  i32 start;
  i32 end;
};

#define lex_token_view_to_str8(buffer, view) (str8) { \
  (const char*) ((u8*)buffer + view.start), \
  view.end \
}

struct lex_token_s
{
  lex_token_type_t token_type;
  lex_token_view_t lexeme;
};

struct lexer_s
{
  const char* buffer;
  i32 start;
  i32 curr;
  i32 end;
};

// TODO(calco): Kinda redundant to have 2 functions but eh
b8 lexer_estimate_token_count(lexer_t* lexer, i32* out_cnt);
void lexer_tokenize(lexer_t* lexer, lex_token_t* out_tokens);
lex_token_t lexer_get_next_token(lexer_t* lexer);

#endif