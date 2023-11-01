#ifndef PARSER_H
#define PARSER_H

#include "defines.h"
#include "str8.h"
#include "lexer.h"

typedef struct parser_node_s parser_node_t;
typedef struct parser_s parser_t;

typedef enum
{
  NODE_TYPE_NUMBER,
  NODE_TYPE_PLUS,
  NODE_TYPE_MINUS,
  NODE_TYPE_MULT,
  NODE_TYPE_DIV,
  NODE_TYPE_EXP,
  NODE_TYPE_SQRT,
  NODE_TYPE_LOG,
  NODE_TYPE_PAREN_GROUP
} parser_node_type_t;

struct parser_node_s
{
  parser_node_type_t node_type;
  parser_node_t* parent;

  union value {
    f32 numeric_value;
    struct unary
    {
      parser_node_type_t* down;
    } unary;
    struct binary
    {
      parser_node_type_t* left;
      parser_node_type_t* right;
    } binary;
  } value;
};

struct parser_s
{
  lex_token_t* tokens;
  i32 token_count;
};

#define parser_make(token_buf, token_cnt) (parser_t) { token_buf, token_cnt };

parser_node_t* parser_parse(parser_t* parser, parser_node_t* nodes, i32 count);

#endif