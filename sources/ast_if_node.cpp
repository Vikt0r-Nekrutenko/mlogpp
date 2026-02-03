#include "ast_if_node.hpp"

mlogpp::AST_IfNode::AST_IfNode(const Token &t)
    : AST_Node(t) {}

std::string mlogpp::AST_IfNode::outMlogCode(std::ostream &stream)
{
    std::string leftValue = leftNodeOutMlogCode(stream);
    stream << "jump " << label << " notEqual "  << leftValue << " true" << std::endl;

    AST_Node::outMlogCode(stream);
    stream << label1 << ":" << std::endl;
    return "";
}

