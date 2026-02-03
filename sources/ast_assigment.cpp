#include "ast_assigment.hpp"

mlogpp::AST_Assigment::AST_Assigment(const Token &t)
    :ASTNode(t) {}

std::string mlogpp::AST_Assigment::outMlogCode(std::ostream &stream)
{
    std::string value = rightNodeOutMlogCode(stream);
    stream << "set " << left->token.value() << " " << value << std::endl;
    return left->token.value();
}
