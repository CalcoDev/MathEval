#include "parser.h"

// TODO(calco): THIS SHOULD NOT BE GLOBAL
static parser_node_t* _out_nodes;

parser_node_t* parser_alloc_node(parser_t* parser)
{
  static i32 node_idx = 0;

  // TODO(calco): FIX TOKEN COUNT THING
  c_assert(node_idx < parser->token_count, 
    "Alloc node index went overboard."
  );

  return &_out_nodes[node_idx++];
}

parser_node_t* parser_parse(parser_t* parser, parser_node_t* out_nodes, i32* count)
{
  _out_nodes = out_nodes;

  // get the left thing
  parser_node_t* lhs = parser_alloc_node(parser);
  *lhs = parser_get_next_node(parser);
  c_assert(lhs->node_type == NODE_TYPE_NUMBER, 
    "Left hand side should always be a number!"
  );

  // If that is all, return that.
  if (parser->token_count < 2)
    return lhs;

  // If more tokens, get the operator. (if not an operator error)
  // Handle the operator.
  parser_node_t* operator = parser_alloc_node(parser);
  *operator = parser_get_next_node(parser);

  b8 is_binary_op = 
    operator->node_type == NODE_TYPE_PLUS || 
    operator->node_type == NODE_TYPE_MINUS ||
    operator->node_type == NODE_TYPE_MULT ||
    operator->node_type == NODE_TYPE_DIV ||
    operator->node_type == NODE_TYPE_EXP;
  c_assert(is_binary_op, "Not a binary operator. Unimplemented yet.");

  // Get right
  parser_node_t* rhs = parser_alloc_node(parser);
  *rhs = parser_get_next_node(parser);
  c_assert(rhs->node_type == NODE_TYPE_NUMBER, 
    "Right hand side should always be a number!"
  );

  // Combine
  operator->as.binary.left = lhs;
  operator->as.binary.right = rhs;
  lhs->parent = operator;
  rhs->parent = operator;

  return operator;
}

parser_node_t parser_make_node(parser_t* parser, parser_node_t* parent, parser_node_type_t type)
{
  return (parser_node_t) {
    .parent = parent,
    .node_type = type,
  };
}


parser_node_t parser_make_numeric(parser_t* parser, f32 value)
{
  return (parser_node_t) {
    .parent = NULL,
    .node_type = NODE_TYPE_NUMBER,
    .as.number = value
  };
}

parser_node_t parser_make_unary(parser_t* parser, parser_node_type_t type)
{
  return (parser_node_t) {
    .parent = NULL,
    .node_type = type,
    .as.unary.down = NULL
  };
}

parser_node_t parser_make_binary(parser_t* parser, parser_node_type_t type)
{
  return (parser_node_t) {
    .parent = NULL,
    .node_type = type,
    .as.binary.left = NULL,
    .as.binary.right = NULL,
  };
}

parser_node_t parser_get_next_node(parser_t* parser)
{
  static i32 token_idx = 0;
  lex_token_t* token = &parser->tokens[token_idx++];

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
      return parser_make_numeric(parser, value);
    case TOKEN_TYPE_PLUS:
    return parser_make_binary(parser, NODE_TYPE_PLUS);
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