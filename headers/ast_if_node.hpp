#ifndef AST_IF_NODE_HPP
#define AST_IF_NODE_HPP

#include "ast_node.hpp"

namespace mlogpp
{

struct AST_IfNode : public ASTNode
{
    AST_IfNode(const mlogpp::Token &t);
    std::string label, label1;

    std::string outMlogCode(std::ostream &stream) override;
};

}

#endif // AST_IF_NODE_HPP
