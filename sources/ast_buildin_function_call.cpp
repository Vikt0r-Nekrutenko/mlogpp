#include "ast_buildin_function_call.hpp"


mlogpp::AST_BuildInFunctionCall::AST_BuildInFunctionCall(const Token &t)
    :ASTNode(t){}

std::string mlogpp::AST_BuildInFunctionCall::outMlogCode(std::ostream &stream)
{
    if(token.value() == "sensor") {
        std::string resultVariable = "_tempVar" + std::to_string(tempVariableN++);
        stream << token.value() << " " << resultVariable << " " << childs.at(0)->token.value() << " "  << childs.at(1)->token.value() << std::endl;
        return resultVariable;
    } else if(token.value() == "control") {
        stream << token.value() << " " << childs.at(1)->token.value() << " "  << childs.at(0)->token.value() << " " << childs.at(2)->token.value() << std::endl;
    }
    return "";
}
