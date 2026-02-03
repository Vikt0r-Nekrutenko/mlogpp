#ifndef AST_VARIABLE_HPP
#define AST_VARIABLE_HPP

#include "ast_node.hpp"

namespace mlogpp
{

struct AST_Variable : public ASTNode
{
    AST_Variable(const Token &t);

    std::string outMlogCode(std::ostream &stream) override;
};

}

#endif // AST_VARIABLE_HPP
