#include "ast_else_node.hpp"


mlogpp::AST_ElseNode::AST_ElseNode(const Token &t)
    : AST_Node(t) {}

std::string mlogpp::AST_ElseNode::outMlogCode(std::ostream &stream)
{
    stream << label << ":" << std::endl;
    AST_Node::outMlogCode(stream);
    stream << label1 << ":" << std::endl;
    return "";
}

