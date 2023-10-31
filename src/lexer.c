#include "lexer.h"

lexer_t lexer_make(str8 buffer)
{
  return (lexer_t) { buffer.buffer, 0, 0, buffer.length };
}

/*
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
TOKEN_TYPE_CLOSE_PAREN
*/

lex_token_type_t lexer_get_token_type(lexer_t* lexer, lex_token_view_t view)
{
  str8 lexeme = (str8) { (lexer->buffer + view.start), view.end - view.start };

  if (lexer->curr >= lexer->end || str8_equals(lexeme, str8_make("\0")))
    return TOKEN_TYPE_EOF;
  
  if (char_is_number(lexeme.buffer[0])) // lexeme.length == 1 && 
    return TOKEN_TYPE_NUMBER;
  
  if (str8_equals(lexeme, str8_make("+")))
    return TOKEN_TYPE_PLUS;
  if (str8_equals(lexeme, str8_make("-")))
    return TOKEN_TYPE_MINUS;
  if (str8_equals(lexeme, str8_make("*")))
    return TOKEN_TYPE_MULT;
  if (str8_equals(lexeme, str8_make("/")))
    return TOKEN_TYPE_DIV;
  if (str8_equals(lexeme, str8_make("^")))
    return TOKEN_TYPE_EXP;
  if (str8_equals(lexeme, str8_make("sqrt")))
    return TOKEN_TYPE_SQRT;
  if (str8_equals(lexeme, str8_make("log")))
    return TOKEN_TYPE_LOG;
  if (str8_equals(lexeme, str8_make("(")))
    return TOKEN_TYPE_OPEN_PAREN;
  if (str8_equals(lexeme, str8_make(")")))
    return TOKEN_TYPE_CLOSE_PAREN;
  
  return TOKEN_TYPE_UNKNOWN_TOKEN;
}

lex_token_t lexer_get_next_token(lexer_t* lexer)
{
  // Skip over whitespace
  for (; lexer->buffer[lexer->start] == ' '; ++lexer->start);

  // Read everything until whitespace
  for (; lexer->buffer[lexer->curr] != ' '; ++lexer->curr);

  lex_token_view_t view = { lexer->start, lexer->curr };
  
  // Prepare for next token
  lexer->start = lexer->curr;
  lexer->curr = lexer->start + 1;
  return (lex_token_t) { lexer_get_token_type(lexer, view), view };
}