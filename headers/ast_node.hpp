#ifndef AST_NODE_HPP
#define AST_NODE_HPP

#include "tokenizer.hpp"
#include <string>
#include <vector>

namespace mlogpp {

struct ASTNode
{
    std::vector<ASTNode*> childs;
    mlogpp::Token token;
    ASTNode *left = nullptr;
    ASTNode *right = nullptr;
    static size_t tempVariableN;

    ASTNode(const mlogpp::Token &t);

    virtual ~ASTNode();

    std::string leftNodeOutMlogCode(std::ostream &stream);

    std::string rightNodeOutMlogCode(std::ostream &stream);

    virtual std::string outMlogCode(std::ostream &stream);

    void printTree(size_t depth) const;
};

}

#endif // AST_NODE_HPP
