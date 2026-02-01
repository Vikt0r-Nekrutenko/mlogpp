#ifndef TOKENIZER_HPP
#define TOKENIZER_HPP

#include <regex>
#include <string>
#include <map>
#include <vector>

namespace mlogpp
{
class SyntaxErrorHandler;

class Token
{
    public:

    enum class Type {
        NotToken,
        Comment,
        Number,
        String,
        Variable,
        Assigment,
        Operator,
        KeywordMlog,
        MlogValue,
        BlockStart,
        BlockEnd,
        Endl,
        KeywordIf,
        KeywordElse,
        CellAccess,
        KeywordFunction,
        FunctionName,
        FunctionCall,
        BuildInFunctionCall,
        ReturnKeyword,
        Parameter,
        Argument,
    };

    std::map<Type, std::string> TypeMap {
        {Type::NotToken, "Not a token!"},
        {Type::Comment, "Comment"},
        {Type::Number, "Number"},
        {Type::String, "String"},
        {Type::Variable, "Variable"},
        {Type::Assigment, "Assigment"},
        {Type::Operator, "Operator"},
        {Type::KeywordMlog, "KeywordMlog"},
        {Type::KeywordIf, "KeywordIf"},
        {Type::KeywordElse, "KeywordElse"},
        {Type::BlockStart, "BlockStart"},
        {Type::BlockEnd, "BlockEnd"},
        {Type::Endl, "Endl"},
        {Type::CellAccess, "CellAccess"},
        {Type::KeywordFunction, "KeywordFunction"},
        {Type::ReturnKeyword, "ReturnKeyword"},
        {Type::FunctionName, "FunctionName"},
        {Type::FunctionCall, "FunctionCall"},
        {Type::BuildInFunctionCall, "BuildInFunctionCall"},
        {Type::Parameter, "Parameter"},
        {Type::Argument, "Argument"},
        {Type::MlogValue, "MlogValue"},
    };

    Token(size_t ln, const std::string &v, Type t);

    inline std::string &value() { return mValue; }
    inline size_t &lineNumber() { return mLineNumber; }
    inline Type &type() { return mType; }

    inline const std::string &value() const { return mValue; }
    inline const size_t &lineNumber() const { return mLineNumber; }
    inline const Type &type() const { return mType; }

    std::string getTypeName();

    std::string info() const;

    std::string getOpName() const;

    int precedence() const;

    friend int tokenize(size_t lineNumber, std::vector<Token> &tokens, const std::string line, SyntaxErrorHandler &seh);

    private:

    std::string mValue;
    size_t mLineNumber;
    Type mType;
};

int tokenize(size_t lineNumber, std::vector<Token> &tokens, const std::string line, SyntaxErrorHandler &seh);
}

#endif // TOKENIZER_HPP
