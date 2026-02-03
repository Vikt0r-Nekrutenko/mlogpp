#ifndef AST_RETURN_NODE_HPP
#define AST_RETURN_NODE_HPP

#include "ast_function_implementation_node.hpp"

namespace mlogpp
{

struct ASTReturnNode : public ASTNode
{
    AST_FunctionImplementationNode* function = nullptr;
    ASTReturnNode(const mlogpp::Token &t);
    std::string outMlogCode(std::ostream &stream) override;
};

}

#endif // AST_RETURN_NODE_HPP
