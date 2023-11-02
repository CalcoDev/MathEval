#include "lexer.h"

#include <ctype.h>

operator_type_t lex_token_get_op_type(lex_token_type_t node)
{
  switch (node)
  {
    case TOKEN_TYPE_NUMBER:
      return OPERATOR_NO;
    case TOKEN_TYPE_PLUS:
      return OPERATOR_BINARY;
    case TOKEN_TYPE_MINUS:
      return OPERATOR_BINARY;
    case TOKEN_TYPE_MULT:
      return OPERATOR_BINARY;
    case TOKEN_TYPE_DIV:
      return OPERATOR_BINARY;
    case TOKEN_TYPE_EXP:
      return OPERATOR_BINARY;
    case TOKEN_TYPE_SQRT:
      return OPERATOR_UNARY;
    case TOKEN_TYPE_LOG:
      return OPERATOR_UNARY;
    case TOKEN_TYPE_OPEN_PAREN:
      return OPERATOR_NO;
    case TOKEN_TYPE_CLOSE_PAREN:
      return OPERATOR_NO;
    case TOKEN_TYPE_EOF:
      return OPERATOR_NO;
    default:
      break;
  }

  c_assert(0, "Unreachable. Token type does not implement operator type.");
  return OPERATOR_NO;
}

b8 lexer_estimate_token_count(lexer_t* lexer, i32* out_cnt)
{
  i32 token_count = 0;
  for (; 1; ++token_count)
  {
    lex_token_t token = lexer_get_next_token(lexer);
    if (token.token_type == TOKEN_TYPE_EOF)
      break;
    
    if (token.token_type == TOKEN_TYPE_UNKNOWN_TOKEN)
    {
      if (out_cnt)
        *out_cnt = token.lexeme.start;

      return -1;
    }
  }

  if (out_cnt)
    *out_cnt = token_count;

  return 1;
}

void lexer_tokenize(lexer_t* lexer, lex_token_t* out_tokens)
{
  lexer->curr = 0;
  lexer->start = 0;

  i32 token_count = 0;
  for (; 1; ++token_count)
  {
    out_tokens[token_count] = lexer_get_next_token(lexer);
    if (out_tokens[token_count].token_type == TOKEN_TYPE_EOF)
      break;
  }
}

lex_token_t lexer_make_token(lexer_t* lexer, lex_token_type_t type)
{
  i32 start = lexer->start;
  i32 curr = lexer->curr;
  
  lexer->start = lexer->curr;
  return (lex_token_t) {
    .token_type = type,
    .lexeme = (lex_token_view_t) {
      .start = start,
      .end = curr
    } 
  };
}

lex_token_t lexer_get_next_token(lexer_t* lexer)
{
  for (; lexer->buffer[lexer->start] == ' '; ++lexer->start);
  lexer->curr = lexer->start + 1;

  // TODO(calco): Replace this with a hashmap / any binary search lol.
  if (lexer->buffer[lexer->start] == '\0')
    return lexer_make_token(lexer, TOKEN_TYPE_EOF);
  
  str8 lexeme = (str8) { 
    (lexer->buffer + lexer->start), 
    lexer->end - lexer->start 
  };

  if (char_is_number(lexeme.buffer[0]))
  {
    for (; isdigit(lexer->buffer[lexer->curr]) || lexer->buffer[lexer->curr] == '.'; ++lexer->curr);
    return lexer_make_token(lexer, TOKEN_TYPE_NUMBER);
  }
  if (str8_starts_with(lexeme, str8_make("sqrt")))
  {
    for (; lexer->buffer[lexer->curr] != ' '; ++lexer->curr)
    {
      if (lexer->buffer[lexer->curr] == '\0')
        c_assert(0, "Unexpected EOF while reading sqrt!");
    }
    return lexer_make_token(lexer, TOKEN_TYPE_SQRT);
  }
  if (str8_starts_with(lexeme, str8_make("log")))
  {
    for (; lexer->buffer[lexer->curr] != ' '; ++lexer->curr)
    {
      if (lexer->buffer[lexer->curr] == '\0')
        c_assert(0, "Unexpected EOF while reading log!");
    }
    return lexer_make_token(lexer, TOKEN_TYPE_LOG);
  }

  if (str8_starts_with(lexeme, str8_make("(")))
    return lexer_make_token(lexer, TOKEN_TYPE_OPEN_PAREN);
  if (str8_starts_with(lexeme, str8_make(")")))
    return lexer_make_token(lexer, TOKEN_TYPE_CLOSE_PAREN);
  if (str8_starts_with(lexeme, str8_make("+")))
    return lexer_make_token(lexer, TOKEN_TYPE_PLUS);
  if (str8_starts_with(lexeme, str8_make("-")))
    return lexer_make_token(lexer, TOKEN_TYPE_MINUS);
  if (str8_starts_with(lexeme, str8_make("*")))
    return lexer_make_token(lexer, TOKEN_TYPE_MULT);
  if (str8_starts_with(lexeme, str8_make("/")))
    return lexer_make_token(lexer, TOKEN_TYPE_DIV);
  if (str8_starts_with(lexeme, str8_make("^")))
    return lexer_make_token(lexer, TOKEN_TYPE_EXP);

  return lexer_make_token(lexer, TOKEN_TYPE_UNKNOWN_TOKEN);
}