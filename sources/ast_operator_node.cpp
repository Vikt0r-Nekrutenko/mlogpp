#include "ast_operator_node.hpp"

mlogpp::AST_OperatorNode::AST_OperatorNode(const Token &t)
    : AST_Node(t) {}

std::string mlogpp::AST_OperatorNode::outMlogCode(std::ostream &stream)
{
    std::string leftValue = leftNodeOutMlogCode(stream);
    std::string rightValue = rightNodeOutMlogCode(stream);
    std::string resultVariable = "_tempVar" + std::to_string(tempVariableN++);
    stream << token.getOpName() << " " << resultVariable << " " << leftValue << " " << rightValue << std::endl;
    return resultVariable;
}
