#include "ast_variable.hpp"


mlogpp::AST_Variable::AST_Variable(const Token &t)
    : ASTNode(t) {}

std::string mlogpp::AST_Variable::outMlogCode(std::ostream &stream)
{
    return token.value();
}
