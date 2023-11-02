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

typedef parser_node_t* getx_bexp_lhs_func(parser_t* parser);
typedef b8 get_bexp_continue_func(lex_token_type_t token_type);
parser_node_t* parse_binary_expression(parser_t* parser, 
  getx_bexp_lhs_func* get_lhs, get_bexp_continue_func* get_bexp_continue)
{
  parser_node_t* lhs = get_lhs(parser);
  if (parser->token_count < 2)
    return lhs;
  parser_node_t* op = NULL;
  parser_node_t* rhs = NULL;

  while (lex_token_get_op_type(parser->lookahead->token_type) == OPERATOR_BINARY
    && get_bexp_continue(parser->lookahead->token_type))
  {
    op = parse_node_operator_type(parser, OPERATOR_BINARY);
    rhs = get_lhs(parser);
    op->as.binary.left = lhs;
    op->as.binary.right = rhs;
    lhs->parent = op;
    rhs->parent = op;
    lhs = op;
  }

  return lhs;
}

parser_node_t* parse_expression(parser_t* parser);
parser_node_t* parse_paren(parser_t* parser)
{
  parse_node_type(parser, TOKEN_TYPE_OPEN_PAREN);
  parser_node_t* node = parse_expression(parser);
  parse_node_type(parser, TOKEN_TYPE_CLOSE_PAREN);
  return node;
}

parser_node_t* parse_numeric_literal(parser_t* parser)
{
  return parse_node_type(parser, TOKEN_TYPE_NUMBER);
}

parser_node_t* parse_primary_expression(parser_t* parser)
{
  if (parser->lookahead->token_type == TOKEN_TYPE_NUMBER)
    return parse_numeric_literal(parser);
  else if (parser->lookahead->token_type == TOKEN_TYPE_OPEN_PAREN)
    return parse_paren(parser);
  
  c_assert(0, "Unreachable. Primary expression was neither parentheses, nor a numeric literal!");
  return NULL;
}

b8 get_exp_expr(lex_token_type_t token_type)
{
  return token_type == TOKEN_TYPE_EXP;
}
parser_node_t* parse_exponent_expressions(parser_t* parser)
{
  return parse_binary_expression(parser, parse_primary_expression, get_exp_expr);
}

b8 get_mult_expr(lex_token_type_t token_type)
{
  return token_type == TOKEN_TYPE_MULT || token_type == TOKEN_TYPE_DIV;
}
parser_node_t* parse_multiplicative_expressions(parser_t* parser)
{
  return parse_binary_expression(parser, parse_exponent_expressions, get_mult_expr);
}

b8 get_add_expr(lex_token_type_t token_type)
{
  return token_type == TOKEN_TYPE_PLUS || token_type == TOKEN_TYPE_MINUS;
}
parser_node_t* parse_additive_expressions(parser_t* parser)
{
  return parse_binary_expression(parser, parse_multiplicative_expressions, get_add_expr);
}

parser_node_t* parse_expression(parser_t* parser)
{
  return parse_additive_expressions(parser);
}

parser_node_t* parser_parse(parser_t* parser, parser_node_t* out_nodes, i32* count)
{
  _out_nodes = out_nodes;
  parser->lookahead = &parser->tokens[0];
  parser_node_t* node = parse_expression(parser);
  _out_nodes = NULL;
  return node;
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
    case TOKEN_TYPE_MINUS:
      return parser_make_binary(parser, TOKEN_TYPE_MINUS);
    case TOKEN_TYPE_MULT:
      return parser_make_binary(parser, TOKEN_TYPE_MULT);
    case TOKEN_TYPE_DIV:
      return parser_make_binary(parser, TOKEN_TYPE_DIV);
    case TOKEN_TYPE_EXP:
      return parser_make_binary(parser, TOKEN_TYPE_EXP);
    case TOKEN_TYPE_SQRT:
      break;
    case TOKEN_TYPE_LOG:
      break;
    case TOKEN_TYPE_OPEN_PAREN:
      return parser_make_binary(parser, TOKEN_TYPE_OPEN_PAREN);
    case TOKEN_TYPE_CLOSE_PAREN:
      return parser_make_binary(parser, TOKEN_TYPE_CLOSE_PAREN);
    case TOKEN_TYPE_UNKNOWN_TOKEN:
      break;
    default:
      break;
  }

  c_assert(0, "Unreachable");
  return (parser_node_t) {0};
}