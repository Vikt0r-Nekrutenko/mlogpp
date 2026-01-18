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
    };

    std::map<Type, std::string> TypeMap {
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
        {Type::KeywordFunction, "FunctionKeyword"},
        {Type::FunctionName, "FunctionName"},
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
int tokenizeStrings(const std::smatch &match, size_t lineNumber, std::vector<Token> &tokens, const std::string line, SyntaxErrorHandler &seh);
int tokenizeKeywords(size_t lineNumber, std::vector<Token> &tokens, const std::string keyword, SyntaxErrorHandler &seh);
int tokenizeOperators(size_t lineNumber, std::vector<Token> &tokens, const std::string buffer, SyntaxErrorHandler &seh);
int tokenizeName(size_t lineNumber, std::vector<Token> &tokens, const std::string name, SyntaxErrorHandler &seh);
}

#endif // TOKENIZER_HPP
