#ifndef AST_ASSIGMENT_HPP
#define AST_ASSIGMENT_HPP

#include "ast_node.hpp"


namespace mlogpp
{

struct AST_Assigment : public ASTNode
{
    AST_Assigment(const Token &t);

    std::string outMlogCode(std::ostream &stream) override;
};

}

#endif // AST_ASSIGMENT_HPP
