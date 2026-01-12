#ifndef TOKENIZER_HPP
#define TOKENIZER_HPP

#include <string>
#include <regex>
#include <map>

namespace mlogpp
{
  class Token
  {
    public:
    
    enum class Type
    {
      Number,
      String,
      Variable,
      Assigment,
      Operator,
    };
    
    std::map<Type, std::string> TypeMap {
      {Type::Number, "Number"},
      {Type::String, "String"},
      {Type::Variable, "Variable"},
      {Type::Assigment, "Assigment"},
      {Type::Operator, "Operator"},
    };
    
    Token(size_t ln, const std::string &v, Type t)
      : mValue{v}, mLineNumber{ln}, mType{t} {}
    
    const std::string getTypeName()
    {
      auto name = TypeMap[mType];
      return name.empty() ? "Undefined" : name;
    }
      
    friend int tokenize(size_t lineNumber, std::vector<Token> &tokens, const std::string line);
      
    private:
    
    std::string mValue;
    size_t mLineNumber;
    Type mType;
  };
  
  int tokenize(size_t lineNumber, std::vector<Token> &tokens, const std::string line)
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
    }
    return 0;
  }
}

#endif // TOKENIZER_HPP