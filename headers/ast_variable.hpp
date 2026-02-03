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

using AST_Number = AST_Variable;
using AST_Argument = AST_Variable;
using AST_Parameter = AST_Variable;

}

#endif // AST_VARIABLE_HPP
