#include "tokenizer.hpp"
#include "syntax_error_handler.hpp"

#define ITS_NOT_A_FUNCTION 0x0
#define ITS_FUNCTION_CALL 0x1
#define ITS_FUNCTION_IMPLEMENTATION 0x2

int functionTokenType = ITS_NOT_A_FUNCTION;

using namespace mlogpp;

std::map<std::string, bool> buildInFunctionsNames {
    {"sensor", true},
    {"control", true},
};

std::map<std::string, bool> functionsNames;

mlogpp::Token::Token(size_t ln, const std::string &v, Type t)
    : mValue{v}, mLineNumber{ln}, mType{t} {}

std::string mlogpp::Token::getTypeName()
{
    auto name = TypeMap[mType];
    return name.empty() ? "Undefined" : name;
}

std::string mlogpp::Token::info() const
{
    std::string info;
    info += std::to_string(mLineNumber) + ": [" + mValue + "] - " + const_cast<Token *>(this)->getTypeName();
    return info;
}

std::string mlogpp::Token::getOpName() const
{
         if(value() == "+") return "op add";
    else if(value() == "-") return "op sub";
    else if(value() == "*") return "op mul";
    else if(value() == "/") return "op div";
    else if(value() == "<") return "op lessThan";
    else if(value() == ">") return "op greaterThan";
    else if(value() == "<=") return "op lessThanEq";
    else if(value() == ">=") return "op greaterThanEq";
    else if(value() == "==") return "op strictEqual";
    else if(value() == "!=") return "op notEqual";
    else if(value() == "and") return "op land";
    else if(value() == "or") return "op or";
    return "";
}

int mlogpp::Token::precedence() const
{
    if(value() == "*" || value() == "/" || value() == "%")
        return 6;
    else if(value() == "+" || value() == "-")
        return 5;
    else if(value() == "<" || value() == ">" || value() == "<=" || value() == ">=")
        return 4;
    else if(value() == "==" || value() == "!=")
        return 3;
    else if(value() == "&" || value() == "|")
        return 2;
    else if(value() == "and" || value() == "or")
        return 1;
    return 0;
}

///-------------------------------------------------------------------------------------------------------------------------------------------

int tokenizeStrings(const std::smatch &match, size_t lineNumber, std::vector<Token> &tokens, const std::string line, SyntaxErrorHandler &seh);
Token::Type tokenizeKeywords(const std::string keyword);
Token::Type tokenizeOperators(std::vector<Token> &tokens, const std::string buffer);
Token::Type tokenizeName(std::vector<Token> &tokens, const std::string name);
Token::Type tokenizeVariableName();
Token::Type tokenizeNumberName();

int mlogpp::tokenize(size_t lineNumber, std::vector<Token> &tokens, const std::string line, SyntaxErrorHandler &seh)
{
    const std::regex pattern(
        R"(\s*("(?:[^"\\]|\\.)*")|)" // strings
        R"((and|or|if|else|mlog|function|return)|)" // keywords
        R"((-?\d+\.\d+|-?\d+)|)" // numbers
        R"(([a-zA-Z_][\w]*)|)" // variables
        R"((\.|\/\/|!=|==|<=|>=|[\;\+\-\/\*\=\(\)\<\>\&\|\%|\{|\}\[\]])|)"); // operators
    auto wordsBegin = std::sregex_iterator(line.begin(), line.end(), pattern);
    auto wordEnd = std::sregex_iterator();
    for(auto i = wordsBegin; i != wordEnd; ++i) {
        std::smatch match = *i;
        if(match[1].matched) { // strings
            tokenizeStrings(match, lineNumber, tokens, line, seh);
        } else if(match[2].matched) { // keywords
            Token::Type type = tokenizeKeywords(match[2].str());
            tokens.push_back({lineNumber, match[2].str(), type});
            seh.checkError(tokens);
        } else if(match[3].matched) { // numbers
            Token::Type type = tokenizeNumberName();
            tokens.push_back({lineNumber, match[3].str(), type});
            seh.checkError(tokens);
        } else if(match[4].matched) { // variables
            Token::Type type = tokenizeName(tokens, match[4].str());
            tokens.push_back({lineNumber, match[4].str(), type});
            seh.checkError(tokens);
        } else if(match[5].matched) { // operators
            Token::Type type = tokenizeOperators(tokens, match[5].str());
            if(type == Token::Type::Comment)
              return 0;
            tokens.push_back({lineNumber, match[5].str(), type});
            seh.checkError(tokens);
        }
    }
    return 0;
}

int tokenizeStrings(const std::smatch &match, size_t lineNumber, std::vector<Token> &tokens, const std::string line, SyntaxErrorHandler &seh)
{
    static std::string string;
    std::string subStr = match[1].str();
    for(auto it = subStr.begin()+1; it != subStr.end()-1; ++it)
        if(*it != '\\')
            string += *it;
    string += '\n';
    if(line.back() == ';') {
        string.pop_back(); // remove last '\n'
        tokens.push_back({lineNumber, string, tokens.back().type() == Token::Type::KeywordMlog ? Token::Type::MlogValue : Token::Type::String});
        string.clear();
        seh.checkError(tokens);
    }
    return 0;
}

Token::Type tokenizeKeywords(const std::string keyword)
{
    if(keyword == "and" || keyword == "or") {
        return mlogpp::Token::Type::Operator;
    } else if(keyword == "if") {
        return mlogpp::Token::Type::KeywordIf;
    } else if(keyword == "else") {
        return mlogpp::Token::Type::KeywordElse;
    } else if(keyword == "mlog") {
        return mlogpp::Token::Type::KeywordMlog;
    } else if(keyword == "function") {
        return mlogpp::Token::Type::KeywordFunction;
    } else if(keyword == "return") {
        return mlogpp::Token::Type::ReturnKeyword;
    }
    return mlogpp::Token::Type::NotToken;
}

Token::Type tokenizeOperators(std::vector<Token> &tokens, const std::string buffer)
{
    if(buffer == "=")
        return Token::Type::Assigment;
    else if(buffer == "{")
      return Token::Type::BlockStart;
    else if(buffer == "}")
      return Token::Type::BlockEnd;
    else if(buffer == ";")
      return Token::Type::Endl;
    else if(buffer == "[") {
      tokens.back().type() = Token::Type::CellAccess;
      return Token::Type::Operator;
    } else if(buffer == ")") {
      if(functionTokenType != ITS_NOT_A_FUNCTION) {
          functionTokenType = ITS_NOT_A_FUNCTION;
      }
    }
    else if(buffer == "//")
      return mlogpp::Token::Type::Comment;
    return mlogpp::Token::Type::Operator;
}

Token::Type tokenizeVariableName()
{
    if(functionTokenType == ITS_FUNCTION_IMPLEMENTATION)
        return mlogpp::Token::Type::Parameter;
    else if(functionTokenType == ITS_FUNCTION_CALL)
        return mlogpp::Token::Type::Argument;
    return mlogpp::Token::Type::Variable;
}

Token::Type tokenizeNumberName()
{
    if(functionTokenType == ITS_FUNCTION_CALL)
        return mlogpp::Token::Type::Parameter;
    else if(functionTokenType == ITS_FUNCTION_IMPLEMENTATION)
        return mlogpp::Token::Type::Argument;
    return mlogpp::Token::Type::Number;
}

Token::Type tokenizeName(std::vector<Token> &tokens, const std::string name)
{
    if(!tokens.empty() && tokens.back().type() == Token::Type::KeywordFunction) {
        functionsNames[name] = true;
        functionTokenType = ITS_FUNCTION_IMPLEMENTATION;
        return Token::Type::FunctionName;
    } else if(buildInFunctionsNames[name] == true) {
        functionTokenType = ITS_FUNCTION_CALL;
        return Token::Type::BuildInFunctionCall;
    } else if(functionsNames[name] == true) {
        functionTokenType = ITS_FUNCTION_CALL;
        return Token::Type::FunctionCall;
    } else {
        return tokenizeVariableName();
    }
    return mlogpp::Token::Type::NotToken;
}
