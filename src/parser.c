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

parser_node_t* parse_node_type(parser_t* parser, lex_token_type_t type)
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
  c_assert(lex_token_get_op_type(parser->lookahead->token_type) == type, 
    "(OPERATOR_TYPE): Expected a %d but got a %d", (i32)type, (i32)parser->lookahead->token_type
  );
  parser_node_t* operator = parser_alloc_node(parser);
  *operator = parser_get_next_node(parser);
  return operator;
}

parser_node_t* parse_additive_expressions(parser_t* parser)
{
  parser_node_t* lhs = parse_node_type(parser, TOKEN_TYPE_NUMBER);
  if (parser->token_count < 2)
    return lhs;
  parser_node_t* op = NULL;
  parser_node_t* rhs = NULL;

  lex_token_type_t lex_type = parser->lookahead->token_type;
  operator_type_t op_type = lex_token_get_op_type(lex_type);
  while (op_type == OPERATOR_BINARY &&
    (lex_type == TOKEN_TYPE_PLUS || lex_type == TOKEN_TYPE_MINUS))
  {
    op = parse_node_operator_type(parser, OPERATOR_BINARY);
    rhs = parse_node_type(parser, TOKEN_TYPE_NUMBER);

    op->as.binary.left = lhs;
    op->as.binary.right = rhs;
    lhs->parent = op;
    rhs->parent = op;

    lhs = op;

    lex_type = parser->lookahead->token_type;
    op_type = lex_token_get_op_type(lex_type);
  }

  return lhs;
}

parser_node_t* parser_parse(parser_t* parser, parser_node_t* out_nodes, i32* count)
{
  _out_nodes = out_nodes;
  return parse_additive_expressions(parser);
}

parser_node_t parser_make_numeric(parser_t* parser, f32 value)
{
  return (parser_node_t) {
    .parent = NULL,
    .node_type = TOKEN_TYPE_NUMBER,
    .as.number = value
  };
}

parser_node_t parser_make_unary(parser_t* parser, lex_token_type_t type)
{
  return (parser_node_t) {
    .parent = NULL,
    .node_type = type,
    .as.unary.down = NULL
  };
}

parser_node_t parser_make_binary(parser_t* parser, lex_token_type_t type)
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

  c_assert(token_idx < parser->token_count, "Token Index is too big!");
  lex_token_t* token = &parser->tokens[token_idx++];

  parser->lookahead = &parser->tokens[token_idx];

  switch (token->token_type)
  {
    case TOKEN_TYPE_EOF:
      return (parser_node_t) {
        .node_type = TOKEN_TYPE_EOF
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
      return parser_make_binary(parser, TOKEN_TYPE_PLUS);
      break;
    case TOKEN_TYPE_MINUS:
      return parser_make_binary(parser, TOKEN_TYPE_MINUS);
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