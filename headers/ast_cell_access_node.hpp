#ifndef AST_CELL_ACCESS_NODE_HPP
#define AST_CELL_ACCESS_NODE_HPP

#include "ast_node.hpp"

namespace mlogpp
{

struct AST_CellAccessNode : public ASTNode
{
    typedef enum _CellAccessType {
        Read, Write,
    } CellAccessType;
    std::string arg;
    CellAccessType accessType;

    AST_CellAccessNode(const mlogpp::Token &t, CellAccessType cat);

    std::string outMlogCode(std::ostream &stream) override;
};

}

#endif // AST_CELL_ACCESS_NODE_HPP
