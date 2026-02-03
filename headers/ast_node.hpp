#ifndef AST_NODE_HPP
#define AST_NODE_HPP

#include "tokenizer.hpp"
#include <string>
#include <vector>

namespace mlogpp {

struct AST_Node
{
    std::vector<AST_Node*> childs;
    mlogpp::Token token;
    AST_Node *left = nullptr;
    AST_Node *right = nullptr;
    static size_t tempVariableN;

    AST_Node(const mlogpp::Token &t);

    virtual ~AST_Node();

    std::string leftNodeOutMlogCode(std::ostream &stream);

    std::string rightNodeOutMlogCode(std::ostream &stream);

    virtual std::string outMlogCode(std::ostream &stream);

    void printTree(size_t depth) const;
};

}

#endif // AST_NODE_HPP
