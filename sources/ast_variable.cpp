#include "ast_variable.hpp"


mlogpp::AST_Variable::AST_Variable(const Token &t)
    : AST_Node(t) {}

mlogpp::AST_Variable::AST_Variable(size_t lineNumber, const std::string &value)
    : AST_Node({lineNumber, value, Token::Type::Variable}) {}

std::string mlogpp::AST_Variable::outMlogCode(std::ostream &stream)
{
    return token.value();
}

mlogpp::AST_String::AST_String(const Token &t)
    :AST_Node(t) {}

std::string mlogpp::AST_String::outMlogCode(std::ostream &stream)
{
    return "\"" + token.value() + "\"";
}
