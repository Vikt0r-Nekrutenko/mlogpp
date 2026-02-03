#include "ast_operator_node.hpp"

mlogpp::ASTOperatorNode::ASTOperatorNode(const Token &t)
    : ASTNode(t) {}

std::string mlogpp::ASTOperatorNode::outMlogCode(std::ostream &stream)
{
    std::string leftValue = leftNodeOutMlogCode(stream);
    std::string rightValue = rightNodeOutMlogCode(stream);
    std::string resultVariable = "_tempVar" + std::to_string(tempVariableN++);
    stream << token.getOpName() << " " << resultVariable << " " << leftValue << " " << rightValue << std::endl;
    return resultVariable;
}
