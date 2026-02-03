#ifndef AST_ELSE_NODE_HPP
#define AST_ELSE_NODE_HPP

#include "ast_node.hpp"

namespace mlogpp
{

struct AST_ElseNode : public ASTNode
{
    AST_ElseNode(const mlogpp::Token &t);
    std::string label, label1;

    std::string outMlogCode(std::ostream &stream) override;
};

}

#endif // AST_ELSE_NODE_HPP
