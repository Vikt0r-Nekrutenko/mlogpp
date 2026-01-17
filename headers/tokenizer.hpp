#ifndef TOKENIZER_HPP
#define TOKENIZER_HPP

#include <string>
#include <map>
#include <vector>

namespace mlogpp
{
  class SyntaxErrorHandler;
  
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
      KeywordMlog,
      BlockStart,
      BlockEnd, 
      Endl, 
      KeywordIf,
      KeywordElse,
      CellAccess
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
      {Type::CellAccess, "Cell"},
    };
    
    Token(size_t ln, const std::string &v, Type t);

    const size_t line_number;
    const std::string value;
    const Type type;
    
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
