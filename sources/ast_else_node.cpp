#include "ast_else_node.hpp"


mlogpp::AST_ElseNode::AST_ElseNode(const Token &t)
    : ASTNode(t) {}

std::string mlogpp::AST_ElseNode::outMlogCode(std::ostream &stream)
{
    stream << label << ":" << std::endl;
    ASTNode::outMlogCode(stream);
    stream << label1 << ":" << std::endl;
    return "";
}

