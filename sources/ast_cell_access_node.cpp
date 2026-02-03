#include "ast_cell_access_node.hpp"

mlogpp::AST_CellAccessNode::AST_CellAccessNode(const Token &t, CellAccessType cat)
    : AST_Node(t), accessType{cat} {
    arg = "_" + t.value() + "Var";
}

std::string mlogpp::AST_CellAccessNode::outMlogCode(std::ostream &stream)
{
    std::string rvalue = rightNodeOutMlogCode(stream);
    std::string lvalue = leftNodeOutMlogCode(stream);
    if(left)
        stream << "set " << arg << " " << left->left->token.value() << std::endl;
    stream << (accessType == Read ? "read " : "write ") << arg << " " << token.value() << " " << rvalue << std::endl;
    return arg;
}
