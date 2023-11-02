#include "parser.h"

operator_type_t parser_node_get_op_type(parser_node_t node)
{
  switch (node.node_type)
  {
    case NODE_TYPE_NUMBER:
      return OPERATOR_NO;
    case NODE_TYPE_PLUS:
      return OPERATOR_BINARY;
    case NODE_TYPE_MINUS:
      return OPERATOR_BINARY;
    case NODE_TYPE_MULT:
      return OPERATOR_BINARY;
    case NODE_TYPE_DIV:
      return OPERATOR_BINARY;
    case NODE_TYPE_EXP:
      return OPERATOR_BINARY;
    case NODE_TYPE_SQRT:
      return OPERATOR_UNARY;
    case NODE_TYPE_LOG:
      return OPERATOR_UNARY;
    case NODE_TYPE_EOF:
      return OPERATOR_NO;
  }

  c_assert(0, "Unreachable. Node type does not implement operator type.");
  return OPERATOR_NO;
}

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

parser_node_t* parse_node_type(parser_t* parser, parser_node_type_t type)
{
  parser_node_t* node = parser_alloc_node(parser);
  *node = parser_get_next_node(parser);
  c_assert(node->node_type == type, 
    "(NODE_TYPE): Expected a %d but got a %d!", (i32)type, (i32)node->node_type
  );
  return node;
}

parser_node_t* parse_node_operator_type(parser_t* parser, operator_type_t type)
{
  parser_node_t* operator = parser_alloc_node(parser);
  *operator = parser_get_next_node(parser);
  c_assert(parser_node_get_op_type(*operator) == type, 
    "(OPERATOR_TYPE): Expected a %d but got a %d", (i32)type, (i32)operator->node_type
  );
  return operator;
}

parser_node_t* parse_additive_expressions(parser_t* parser)
{
  parser_node_t* lhs = parse_node_type(parser, NODE_TYPE_NUMBER);
  if (parser->token_count < 2)
    return lhs;
  parser_node_t* op = parse_node_operator_type(parser, OPERATOR_BINARY);
  parser_node_t* rhs = parse_node_type(parser, NODE_TYPE_NUMBER);

  op->as.binary.left = lhs;
  op->as.binary.right = rhs;
  lhs->parent = op;
  rhs->parent = op;

  return op;
}

parser_node_t* parser_parse(parser_t* parser, parser_node_t* out_nodes, i32* count)
{
  _out_nodes = out_nodes;
  return parse_additive_expressions(parser);
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