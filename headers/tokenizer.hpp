#ifndef TOKENIZER_HPP
#define TOKENIZER_HPP

#include <string>
#include <regex>
#include <map>
#include <iostream>

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
    
    std::string getTypeName()
    {
      auto name = TypeMap[mType];
      return name.empty() ? "Undefined" : name;
    }
    
    void info() const
    {
      std::cout << mLineNumber << ": [" << mValue << "] - " << const_cast<Token *>(this)->getTypeName() << std::endl; 
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
        }
      } 
    }
    return 0;
  }
}

#endif // TOKENIZER_HPP