#ifndef AST_OPERATOR_NODE_HPP
#define AST_OPERATOR_NODE_HPP

#include "ast_node.hpp"


namespace mlogpp
{

struct ASTOperatorNode : public ASTNode
{
    ASTOperatorNode(const mlogpp::Token &t);

    std::string outMlogCode(std::ostream &stream) override;
};

}

#endif // AST_OPERATOR_NODE_HPP
