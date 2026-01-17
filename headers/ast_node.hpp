#ifndef AST_NODE_HPP
#define AST_NODE_HPP

#include <string>
#include <vector>

struct Token;

struct ASTNode
{
    Token token;
    ASTNode *left = nullptr;
    ASTNode *right = nullptr;
    static size_t tempVariableN;

    ASTNode(const Token &t);

    std::string leftNodeOutMlogCode(std::ostream &stream);

    std::string rightNodeOutMlogCode(std::ostream &stream);

    virtual std::string outMlogCode(std::ostream &stream);
};

size_t ASTNode::tempVariableN = 0;

struct ASTOperatorNode : public ASTNode
{
    ASTOperatorNode(const Token &t);

    std::string outMlogCode(std::ostream &stream) override;
};

struct ASTBlock : public ASTNode
{
    ASTBlock();
    std::vector<ASTNode*> childs;

    std::string outMlogCode(std::ostream &stream) override;
};

struct ASTIfBlock : public ASTBlock
{
    ASTIfBlock();
    std::string label, label1;

    std::string outMlogCode(std::ostream &stream) override;
};

struct ASTFunctionImplementationBlock : public ASTBlock
{
    ASTFunctionImplementationBlock(const std::string &name);
};

struct ASTElseBlock : public ASTBlock
{
    ASTElseBlock();
    std::string label, label1;

    std::string outMlogCode(std::ostream &stream) override;
};

struct ASTMlogNode : public ASTNode
{
    ASTMlogNode(const std::string &v);

    std::string outMlogCode(std::ostream &stream) override;
};

struct ASTCellAccessNode : public ASTNode
{
    typedef enum _CellAccessType {
        Read, Write,
    } CellAccessType;
    std::string argumentValue;
    CellAccessType accessType;

    ASTCellAccessNode(const std::string &v, const std::string &av, CellAccessType t);

    std::string outMlogCode(std::ostream &stream) override;
};

#endif // AST_NODE_HPP
