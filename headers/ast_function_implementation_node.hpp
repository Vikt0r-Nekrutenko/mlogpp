#ifndef AST_FUNCTION_IMPLEMENTATION_NODE_HPP
#define AST_FUNCTION_IMPLEMENTATION_NODE_HPP

#include "ast_node.hpp"

namespace mlogpp
{

struct AST_FunctionImplementationNode : public AST_Node
{
    std::vector<mlogpp::Token> params;
    std::string label = "ENDFUNCTION_";
    AST_FunctionImplementationNode(const mlogpp::Token &t);
    std::string outMlogCode(std::ostream &stream) override;
};

}

#endif // AST_FUNCTION_IMPLEMENTATION_NODE_HPP
