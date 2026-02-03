#include "ast_function_implementation_node.hpp"

mlogpp::ASTFunctionImplementationBlock::ASTFunctionImplementationBlock(const Token &t)
    : ASTNode(t) {}

std::string mlogpp::ASTFunctionImplementationBlock::outMlogCode(std::ostream &stream)
{
    for(auto ch : childs)
        ch->outMlogCode(stream);
    stream << label << ":" << std::endl;
    return childs.back()->left->token.value();
}
