#include "tokenizer.hpp"
#include "syntax_error_handler.hpp"


mlogpp::Token::Token(size_t ln, const std::string &v, Type t)
    : line_number(ln), value(v), type(t), mValue{v}, mLineNumber{ln}, mType{t} {}

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

int mlogpp::tokenize(size_t lineNumber, std::vector<Token> &tokens, const std::string line, SyntaxErrorHandler &seh)
{
    const std::regex pattern(
        R"(\s*("(?:[^"\\]|\\.)*")|)" // strings
        R"((and|or|if|else|mlog|cell\d+)|)" // keywords
        R"((-?\d+\.\d+|-?\d+)|)" // numbers
        R"(([a-zA-Z_][\w]*)|)" // variables
        R"((!=|==|<=|>=|[\;\+\-\/\*\=\(\)\<\>\&\|\%|\{|\}\[\]])|)" // operators
        );
    auto wordsBegin = std::sregex_iterator(line.begin(), line.end(), pattern);
    auto wordEnd = std::sregex_iterator();
    for(auto i = wordsBegin; i != wordEnd; ++i) {
        std::smatch match = *i;
        if(match[1].matched) { // strings
            static std::string string;
            std::string subStr = match[1].str();
            for(auto it = subStr.begin()+1; it != subStr.end()-1; ++it)
                if(*it != '\\')
                    string += *it;
            string += '\n';
            if(line.back() == ';') {
                string.pop_back(); // remove last '\n'
                tokens.push_back({lineNumber, string, Token::Type::String});
                string.clear();
                seh.checkError(tokens);
            }
        } else if(match[2].matched) { // keywords
            std::string keyword = match[2].str();
            if(keyword == "and" || keyword == "or") {
                tokens.push_back({lineNumber, keyword, Token::Type::Operator});
            } else if(keyword == "if") {
                tokens.push_back({lineNumber, keyword, Token::Type::KeywordIf});
            } else if(keyword == "else") {
                tokens.push_back({lineNumber, keyword, Token::Type::KeywordElse});
            } else if(keyword == "mlog") {
                tokens.push_back({lineNumber, keyword, Token::Type::KeywordMlog});
            } else if(keyword.length() > 4 && std::string(keyword.begin(), keyword.begin()+4) == "cell") {
                tokens.push_back({lineNumber, keyword, Token::Type::Cell});
            }
            seh.checkError(tokens);
        } else if(match[3].matched) { // numbers
            tokens.push_back({lineNumber, match[3].str(), Token::Type::Number});
            seh.checkError(tokens);
        } else if(match[4].matched) { // variables
            tokens.push_back({lineNumber, match[4].str(), Token::Type::Variable});
            seh.checkError(tokens);
        } else if(match[5].matched) { // operators
            std::string buffer = match[5].str();
            if(buffer == "=") {
                tokens.push_back({lineNumber, buffer, Token::Type::Assigment});
            } else if(buffer == "{") {
                tokens.push_back({lineNumber, buffer, Token::Type::BlockStart});
            } else if(buffer == "}") {
                tokens.push_back({lineNumber, buffer, Token::Type::BlockEnd});
            } else if(buffer == ";") {
                tokens.push_back({lineNumber, buffer, Token::Type::Endl});
            } else {
                tokens.push_back({lineNumber, buffer, Token::Type::Operator});
            }
            seh.checkError(tokens);
        }
    }
    return 0;
}
