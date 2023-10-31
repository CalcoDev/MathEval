#include "lexer.h"

lexer_t lexer_make(str8 buffer)
{
  return (lexer_t) { buffer.buffer, 0, 0, buffer.length };
}

lex_token_t lexer_make_token(lexer_t* lexer, lex_token_type_t type)
{
  lexer->start = lexer->curr;
  return (lex_token_t) { 
    type, 
    (lex_token_view_t) { 
      lexer->start, 
      lexer->curr 
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
    for (; lexer->buffer[lexer->curr] != ' '; ++lexer->curr);
    return lexer_make_token(lexer, TOKEN_TYPE_NUMBER);
  }
  if (str8_starts_with(lexeme, str8_make("sqrt")))
  {
    for (; lexer->buffer[lexer->curr] != ' '; ++lexer->curr);
    return lexer_make_token(lexer, TOKEN_TYPE_SQRT);
  }
  if (str8_starts_with(lexeme, str8_make("log")))
  {
    for (; lexer->buffer[lexer->curr] != ' '; ++lexer->curr);
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