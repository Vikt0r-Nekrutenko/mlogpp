#ifndef AST_RETURN_NODE_HPP
#define AST_RETURN_NODE_HPP

#include "ast_function_implementation_node.hpp"

namespace mlogpp
{

struct AST_ReturnNode : public AST_Node
{
    AST_FunctionImplementationNode* function = nullptr;
    AST_ReturnNode(const mlogpp::Token &t);
    std::string outMlogCode(std::ostream &stream) override;
};

}

#endif // AST_RETURN_NODE_HPP
