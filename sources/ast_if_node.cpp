#include "ast_if_node.hpp"

mlogpp::ASTIfBlock::ASTIfBlock(const Token &t)
    : ASTNode(t) {}

std::string mlogpp::ASTIfBlock::outMlogCode(std::ostream &stream)
{
    std::string leftValue = leftNodeOutMlogCode(stream);
    stream << "jump " << label << " notEqual "  << leftValue << " true" << std::endl;

    ASTNode::outMlogCode(stream);
    stream << label1 << ":" << std::endl;
    return "";
}

