#ifndef AST_NODE_HPP
#define AST_NODE_HPP

#include <string>
#include <vector>

enum class Type {Variable, Number, Operator, Assigment, KeywordMlog, MultyString, BlockStart, BlockEnd, Endl, KeywordIf, KeywordElse, Cell, FunctionImplementation, FunctionName, FunctionCall};

struct Token
{
    std::string value;
    Type type;

    int precedence() const
    {
        if(value == "*" || value == "/" || value == "%")
            return 6;
        else if(value == "+" || value == "-")
            return 5;
        else if(value == "<" || value == ">" || value == "<=" || value == ">=")
            return 4;
        else if(value == "==" || value == "!=")
            return 3;
        else if(value == "&" || value == "|")
            return 2;
        else if(value == "and" || value == "or")
            return 1;
        return 0;
    }

    std::string typeName() const
    {
        switch(type) {
        case Type::Variable: return "Variable";
        case Type::Number: return "Number";
        case Type::Assigment: return "Assigment";
        case Type::Operator: return "Operator";
        case Type::BlockStart: return "BlockStart";
        case Type::BlockEnd: return "BlockEnd";
        case Type::Endl: return "EndLine";
        case Type::KeywordIf: return "KeywordIf";
        case Type::KeywordElse: return "KeywordElse";
        case Type::KeywordMlog: return "KeywordMlog";
        case Type::MultyString: return "MultyString";
        case Type::Cell: return "Cell";
        case Type::FunctionImplementation: return "FunctionImplementation";
        case Type::FunctionName: return "FunctionName";
        case Type::FunctionCall: return "FunctionCall";
        default: return "None";
        }
    }

    std::string getOpName() const
    {
        if(value == "+") return "op add";
        else if(value == "-") return "op sub";
        else if(value == "*") return "op mul";
        else if(value == "/") return "op div";
        else if(value == "<") return "op lessThan";
        else if(value == ">") return "op greaterThan";
        else if(value == "<=") return "op lessThanEq";
        else if(value == ">=") return "op greaterThanEq";
        else if(value == "==") return "op strictEqual";
        else if(value == "!=") return "op notEqual";
        else if(value == "and") return "op land";
        else if(value == "or") return "op or";
        return "";
    }
};


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
