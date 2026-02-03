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

struct ASTOperatorNode : public ASTNode
{
    ASTOperatorNode(const mlogpp::Token &t);

    std::string outMlogCode(std::ostream &stream) override;
};

struct ASTIfBlock : public ASTNode
{
    ASTIfBlock(const mlogpp::Token &t);
    std::string label, label1;

    std::string outMlogCode(std::ostream &stream) override;
};

struct ASTFunctionImplementationBlock : public ASTNode
{
    std::vector<mlogpp::Token> params;
    std::string label = "ENDFUNCTION_";
    ASTFunctionImplementationBlock(const mlogpp::Token &t);
    std::string outMlogCode(std::ostream &stream) override;
};

struct ASTElseBlock : public ASTNode
{
    ASTElseBlock(const mlogpp::Token &t);
    std::string label, label1;

    std::string outMlogCode(std::ostream &stream) override;
};

struct ASTCellAccessNode : public ASTNode
{
    typedef enum _CellAccessType {
        Read, Write,
    } CellAccessType;
    std::string arg;
    CellAccessType accessType;

    ASTCellAccessNode(const mlogpp::Token &t, CellAccessType cat);

    std::string outMlogCode(std::ostream &stream) override;
};

struct ASTReturnNode : public ASTNode
{
    ASTFunctionImplementationBlock* function = nullptr;
    ASTReturnNode(const mlogpp::Token &t);
    std::string outMlogCode(std::ostream &stream) override;
};

}

#endif // AST_NODE_HPP
