#ifndef AST_BUILDIN_FUNCTION_CALL_HPP
#define AST_BUILDIN_FUNCTION_CALL_HPP

#include "ast_node.hpp"

namespace mlogpp
{

struct AST_BuildInFunctionCall : public AST_Node
{
    AST_BuildInFunctionCall(const Token &t);

    std::string outMlogCode(std::ostream &stream) override;
};

}

#endif // AST_BUILDIN_FUNCTION_CALL_HPP
