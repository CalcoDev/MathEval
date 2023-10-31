#include "parser.h"

parser_node_t* parser_parse(parser_t* parser, parser_node_t* nodes, i32 count)
{
  parser_node_t* t = nodes[0];

  i32 node_idx = 1;
  i32 token_idx = 0;
  for (; token_idx < parser->token_count; ++token_idx)
  {
    if (parser->tokens[token_idx] == TOKEN_TYPE_EOF)
      break;

    parser_node_t* curr = nodes[node_idx];
    curr->
  }
}