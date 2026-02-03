#include "ast_variable.hpp"


mlogpp::AST_Variable::AST_Variable(const Token &t)
    : ASTNode(t) {}

mlogpp::AST_Variable::AST_Variable(size_t lineNumber, const std::string &value)
    : ASTNode({lineNumber, value, Token::Type::Variable}) {}

std::string mlogpp::AST_Variable::outMlogCode(std::ostream &stream)
{
    return token.value();
}
