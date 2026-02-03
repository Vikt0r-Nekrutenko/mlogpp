#include "ast_else_node.hpp"


mlogpp::ASTElseBlock::ASTElseBlock(const Token &t)
    : ASTNode(t) {}

std::string mlogpp::ASTElseBlock::outMlogCode(std::ostream &stream)
{
    stream << label << ":" << std::endl;
    ASTNode::outMlogCode(stream);
    stream << label1 << ":" << std::endl;
    return "";
}

