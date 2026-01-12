#ifndef TOKENIZER_HPP
#define TOKENIZER_HPP

#include <string>

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
      Function,
    };
    
    Token(size_t ln, const std::string &v, Type t)
      : mValue{v}, mLineNumber{ln}, mType{t} {}
      
    int tokenize(size_t lineNumber, std::vector<Token> &tokens, const std::string line)
    {
      
      return 0;
    }
      
    private:
    
    std::string mValue;
    size_t mLineNumber;
    Type mType;
  };
}

#endif // TOKENIZER_HPP