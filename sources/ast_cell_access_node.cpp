#include "ast_cell_access_node.hpp"

mlogpp::ASTCellAccessNode::ASTCellAccessNode(const Token &t, CellAccessType cat)
    : ASTNode(t), accessType{cat} {
    arg = "_" + t.value() + "Var";
}

std::string mlogpp::ASTCellAccessNode::outMlogCode(std::ostream &stream)
{
    std::string rvalue = rightNodeOutMlogCode(stream);
    std::string lvalue = leftNodeOutMlogCode(stream);
    if(left)
        stream << "set " << arg << " " << left->left->token.value() << std::endl;
    stream << (accessType == Read ? "read " : "write ") << arg << " " << token.value() << " " << rvalue << std::endl;
    return arg;
}
