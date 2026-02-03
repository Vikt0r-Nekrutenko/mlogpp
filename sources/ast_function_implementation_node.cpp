#include "ast_function_implementation_node.hpp"

mlogpp::AST_FunctionImplementationNode::AST_FunctionImplementationNode(const Token &t)
    : ASTNode(t) {}

std::string mlogpp::AST_FunctionImplementationNode::outMlogCode(std::ostream &stream)
{
    for(auto ch : childs)
        ch->outMlogCode(stream);
    stream << label << ":" << std::endl;
    return childs.back()->left->token.value();
}
