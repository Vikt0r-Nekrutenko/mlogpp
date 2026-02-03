#include "ast_return_node.hpp"

mlogpp::ASTReturnNode::ASTReturnNode(const Token &t)
    : ASTNode(t) {}

std::string mlogpp::ASTReturnNode::outMlogCode(std::ostream &stream)
{
    std::string lvalue = leftNodeOutMlogCode(stream);
    std::string value = rightNodeOutMlogCode(stream);
    stream << "set " << lvalue << " " << value << std::endl;
    stream << "jump " << function->label << " always" << std::endl;
    return left->token.value();
}
