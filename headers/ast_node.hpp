#ifndef AST_NODE_HPP
#define AST_NODE_HPP

#include "tokenizer.hpp"
#include <string>
#include <vector>

struct ASTNode
{
    std::vector<ASTNode*> childs;
    mlogpp::Token token;
    ASTNode *left = nullptr;
    ASTNode *right = nullptr;
    static size_t tempVariableN;

    ASTNode(const mlogpp::Token &t);

    std::string leftNodeOutMlogCode(std::ostream &stream);

    std::string rightNodeOutMlogCode(std::ostream &stream);

    virtual std::string outMlogCode(std::ostream &stream);
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
    ASTFunctionImplementationBlock(const mlogpp::Token &t);
};

struct ASTElseBlock : public ASTNode
{
    ASTElseBlock(const mlogpp::Token &t);
    std::string label, label1;

    std::string outMlogCode(std::ostream &stream) override;
};

struct ASTMlogNode : public ASTNode
{
    ASTMlogNode(const mlogpp::Token &t);

    std::string outMlogCode(std::ostream &stream) override;
};

struct ASTCellAccessNode : public ASTNode
{
    typedef enum _CellAccessType {
        Read, Write,
    } CellAccessType;
    std::string argumentValue;
    CellAccessType accessType;

    ASTCellAccessNode(const mlogpp::Token &t, const std::string &av, CellAccessType cat);

    std::string outMlogCode(std::ostream &stream) override;
};

#endif // AST_NODE_HPP
