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
  NODE_TYPE_PAREN_GROUP,
  NODE_TYPE_EOF
} parser_node_type_t;

struct parser_node_s
{
  parser_node_type_t node_type;
  parser_node_t* parent;

  union {
    f32 number;
    struct
    {
      parser_node_type_t* down;
    } unary;
    struct
    {
      parser_node_type_t* left;
      parser_node_type_t* right;
    } binary;
  } as;
};

struct parser_s
{
  const char* buffer;
  lex_token_t* tokens;
  i32 token_count;
};

parser_node_t* parser_parse(parser_t* parser, parser_node_t* out_nodes, i32* count);
parser_node_t parser_get_next_token(parser_t* parser);

#endif