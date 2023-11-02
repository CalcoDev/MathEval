#include "parser.h"

parser_node_t* parser_alloc_token(parser_t* parser)
{
  static i32 node_idx = 0;

  c_assert(node_idx < parser->token_count, 
    "Alloc node index went overboard."
  );

  return &parser->tokens[node_idx++];
}

static i32 token_idx = 0;
parser_node_t* parser_parse(parser_t* parser, parser_node_t* out_nodes, i32* count)
{
  // get the left thing
  parser_node_t* lhs = parser_alloc_token(parser);
  *lhs = parser_get_next_token(parser);

  // If that is all, return that.
  if (parser->token_count < 2)
    return lhs;

  c_assert(0, "Unreachable");
  // If more tokens, get the operator. (if not an operator error)
  // Handle the operator.

  // Combine again

  return NULL;
}

parser_node_t parser_make_node(parser_t* parser, parser_node_t* parent, parser_node_type_t type)
{
  return (parser_node_t) {
    .parent = parent,
    .node_type = type,
  };
}


parser_node_t parser_make_numeric(parser_t* parser, parser_node_t* parent, f32 value)
{
  return (parser_node_t) {
    .parent = parent,
    .node_type = NODE_TYPE_NUMBER,
    .as.number = value
  };
}

parser_node_t parser_make_unary(parser_t* parser, parser_node_t* parent, parser_node_type_t type)
{
  return (parser_node_t) {
    .parent = parent,
    .node_type = type,
    .as.unary.down = NULL
  };
}

parser_node_t parser_make_binary(parser_t* parser, parser_node_t* parent, parser_node_type_t type)
{
  return (parser_node_t) {
    .parent = parent,
    .node_type = type,
    .as.binary.left = NULL,
    .as.binary.right = NULL,
  };
}

parser_node_t parser_get_next_token(parser_t* parser)
{
  lex_token_t* token = &parser->tokens[token_idx];
  switch (token->token_type)
  {
    case TOKEN_TYPE_EOF:
      return (parser_node_t) {
        .node_type = NODE_TYPE_EOF
      };
    case TOKEN_TYPE_NUMBER:
      f32 value;
      me_assert(
        str8_to_number(
          lex_token_view_to_str8(parser->buffer, token->lexeme), 
          &value
        ),
        1,
        "Error parsing numeric token!"
      );
      return parser_make_numeric(parser, NULL, value);
    case TOKEN_TYPE_PLUS:
      break;
    case TOKEN_TYPE_MINUS:
      break;
    case TOKEN_TYPE_MULT:
      break;
    case TOKEN_TYPE_DIV:
      break;
    case TOKEN_TYPE_EXP:
      break;
    case TOKEN_TYPE_SQRT:
      break;
    case TOKEN_TYPE_LOG:
      break;
    case TOKEN_TYPE_OPEN_PAREN:
      break;
    case TOKEN_TYPE_CLOSE_PAREN:
      break;
    case TOKEN_TYPE_UNKNOWN_TOKEN:
      break;
    default:
      break;
  }

  c_assert(0, "Unreachable");
  return (parser_node_t) {0};
}