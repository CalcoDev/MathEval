#ifndef PARSER_H
#define PARSER_H

#include "defines.h"
#include "str8.h"
#include "lexer.h"

typedef struct parser_node_s parser_node_t;
typedef struct parser_s parser_t;

struct parser_node_s
{
  lex_token_type_t node_type;
  parser_node_t* parent;

  union {
    f32 number;
    struct
    {
      parser_node_t* down;
    } unary;
    struct
    {
      parser_node_t* left;
      parser_node_t* right;
    } binary;
  } as;
};

struct parser_s
{
  const char* buffer;
  lex_token_t* tokens;
  i32 token_count;

  lex_token_t* lookahead;
};

parser_node_t* parser_parse(parser_t* parser, parser_node_t* out_nodes, i32* count);
parser_node_t parser_get_next_node(parser_t* parser);

#endif