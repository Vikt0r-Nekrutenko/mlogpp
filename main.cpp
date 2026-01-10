#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <stack>
#include <queue>
#include <regex>

enum class Type {Variable, Number, Operator, Assigment, BlockStart, BlockEnd, Endl, KeywordIf, KeywordElse};

const std::vector<std::string> Keywords {
  "if", "else", "while",
};

struct Token
{
    std::string value;
    Type type;
    
    int precedence() const 
    { 
      if(value == "*" || value == "/")
        return 9;
      else if(value == "+" || value == "-")
        return 8;
      else if(value == "and" || value == "or")
        return 7;
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
        default: return "None";
      }
    }
    
    std::string getOpName() const
    {
             if(value == "+") return "add";
        else if(value == "-") return "sub";
        else if(value == "*") return "mul";
        else if(value == "/") return "div";
        else if(value == "and") return "land";
        else if(value == "or") return "or";
        return "";
    }
};

struct ASTNode
{
    Token token;
    ASTNode *left = nullptr;
    ASTNode *right = nullptr;
    static size_t tempVariableN;

    ASTNode(const Token &t) : token{t} {}
    
    std::string leftNodeOutMlogCode(std::ostream &stream)
    {
      if(left == nullptr)
        return "";
      return left->outMlogCode(stream);
    }
    
    std::string rightNodeOutMlogCode(std::ostream &stream)
    {
      if(right == nullptr)
        return "";
      return right->outMlogCode(stream);
    }
    
    virtual std::string outMlogCode(std::ostream &stream)
    {
      if(token.type == Type::Number || token.type == Type::Variable)
          return token.value;
      if(token.type == Type::Assigment) {
          std::string value = rightNodeOutMlogCode(stream);
          stream << "set " << left->token.value << " " << value << std::endl;
          return left->token.value;
      }
      return "";
    }
};

size_t ASTNode::tempVariableN = 0;

struct ASTOperatorNode : public ASTNode
{
  ASTOperatorNode(const Token &t) : ASTNode(t) {}
  
  std::string outMlogCode(std::ostream &stream) override
  {
    std::string leftValue = leftNodeOutMlogCode(stream);
    std::string rightValue = rightNodeOutMlogCode(stream);
    std::string resultVariable = "T" + std::to_string(tempVariableN++);
    std::cout << "op " << token.getOpName() << " " << resultVariable << " " << leftValue << " " << rightValue << std::endl;
    return resultVariable;
  }
};

struct ASTBlock : public ASTNode
{
  ASTBlock() : ASTNode{Token{"{", Type::BlockStart}} {}
  std::vector<ASTNode*> childs;
  
  std::string outMlogCode(std::ostream &stream) override
  {
    for(auto ch : childs) {
      ch->outMlogCode(stream);
    }
    return "";
  }
};

struct ASTIfBlock : public ASTBlock
{
  ASTIfBlock() : ASTBlock() 
  {
    token.type = Type::KeywordIf;
    token.value = "if";
  }
  std::string label, label1;
  
  std::string outMlogCode(std::ostream &stream) override
  {
    if(token.type == Type::KeywordIf) {
      std::string leftValue = leftNodeOutMlogCode(stream);
      //std::string rightValue = generate(node->right);
      //std::string resultVariable = "T" + std::to_string(mNReg++);
      stream << "jump " << label << " notEqual "  << leftValue << " true" << std::endl;
      //for(auto ch : static_cast<ASTBlock*>(node)->childs) {
      //  generate(ch);
      //}
      ASTBlock::outMlogCode(stream);
      stream << label1 << ":" << std::endl;
    }
    return "";
  }
};

struct ASTElseBlock : public ASTBlock
{
  ASTElseBlock() : ASTBlock() 
  {
    token.type = Type::KeywordElse;
    token.value = "else";
  }
  std::string label, label1;
  
  std::string outMlogCode(std::ostream &stream) override
  {
    if(token.type == Type::KeywordElse) {
      stream << label << ":" << std::endl;
      ASTBlock::outMlogCode(stream);
      stream << label1 << ":" << std::endl;
    }
    return "";
  }
};

Token tokenizeKeywords(const std::string &s)
{
  auto keywordIt = find_if(Keywords.begin(), Keywords.end(), [&](const std::string &keyword) {
    return s == keyword ? true : false;
  });
  if(keywordIt != Keywords.end()) {
    if(*keywordIt == "if")
      return {s, Type::KeywordIf};
    if(*keywordIt == "else")
      return {s, Type::KeywordElse};
  } else if(s == "and" || s == "or") {
    return {s, Type::Operator};
  }
  return {s, Type::Variable};
}

std::vector<Token> tokenize(const std::string &expression)
{
    std::vector<Token> tokens;
    for(size_t i = 0; i < expression.length(); ++i) {
        if(std::isspace(expression[i]))
            continue;
            
        std::string buffer;
        while(i < expression.length() && !std::isalnum(expression[i]) && expression[i] != std::isspace(expression[i])) {
          buffer += expression[i++];
        }
        
        if(!buffer.empty()) {
          if(buffer == "+" || buffer == "-" || buffer == "*" || buffer == "*" || buffer == "/" || buffer == "(" || buffer == ")")
              tokens.push_back({buffer, Type::Operator});
            else if(buffer == "=")
              tokens.push_back({buffer, Type::Assigment});
            else if(buffer == "{")
              tokens.push_back({buffer, Type::BlockStart});
            else if(buffer == "}")
              tokens.push_back({buffer, Type::BlockEnd});
            else if(buffer == ";")
              tokens.push_back({buffer, Type::Endl});
          if(buffer.size() > 1)
            std::cout<<"\t"<<buffer<<std::endl;
          buffer.clear();
          --i;
        }

        while(i < expression.length() && (std::isalnum(expression[i]) || expression[i] == '_')) {
            buffer += expression[i++];
        }
        if(!buffer.empty()) {
          if(std::isdigit(buffer[0])) {
            tokens.push_back({buffer, Type::Number});
          } else {
            tokens.push_back(tokenizeKeywords(buffer));
          }
          i--;
        }
    }
    return tokens;
}

class Parser
{
    std::vector<Token> mTokens;
    std::stack<ASTBlock *> blocks;
    ASTIfBlock *lastIfBlock = nullptr;
    ASTBlock *mainBlock = nullptr;
    size_t mPos = 0;
    size_t mIfLblN = 0;

    Token peek() { return mTokens[mPos]; }

    Token consume() { return mTokens[mPos++]; }

    ASTNode *parseExpression(int minPrec)
    {
        auto left = parsePrimary();
        while(mPos < mTokens.size() && 
          peek().type != Type::Endl &&
          peek().type != Type::BlockStart &&
          peek().type != Type::BlockEnd &&
        mTokens[mPos].precedence() >= minPrec) {
            Token operatr = consume();
            if(operatr.value == ")") {
              break;
            }
            auto node = new ASTOperatorNode(operatr);
            node->left = std::move(left);
            node->right = parseExpression(operatr.precedence() + 1);
            left = std::move(node);
        }
        return left;
    }

    ASTNode *parsePrimary()
    {
        Token t = consume();
        if(t.value == "("){
          return parseExpression(0);
        }
        return new ASTNode(t);
    }

    ASTBlock *lastChildAsBlock()
    {
      return static_cast<ASTBlock*>(mainBlock->childs.back());
    }
    
    template <class T>
    ASTBlock *addNewBlock()
    {
      mainBlock->childs.push_back(new T);
      blocks.push(lastChildAsBlock());
      return blocks.top();
    }
    
    void parseIfKeyword()
    {
      mainBlock = addNewBlock<ASTIfBlock>();
      consume();
      
      mainBlock->left = parseExpression(0);
      consume();
      
      static_cast<ASTIfBlock*>(mainBlock)->label = static_cast<ASTIfBlock*>(mainBlock)->label1 = std::string("ENDIF_") + std::to_string(++mIfLblN);
    }
    
    void parseElseKeyword()
    {
      if(lastChildAsBlock()->token.type == Type::KeywordIf){
          lastIfBlock = static_cast<ASTIfBlock*>(lastChildAsBlock());
        
        mainBlock = addNewBlock<ASTElseBlock>();
        
        static_cast<ASTElseBlock*>(mainBlock)->label1 = lastIfBlock->label;
        lastIfBlock->label1 = "jump " + lastIfBlock->label1 + " always";
        static_cast<ASTElseBlock*>(mainBlock)->label = lastIfBlock->label = std::string("ELSE_") + std::to_string(mIfLblN);
        }
        consume();
    }
    
    void parseBlockOpen()
    {
      if(mainBlock != nullptr) {
        mainBlock = addNewBlock<ASTBlock>();
      } else {
        mainBlock = new ASTBlock;
          blocks.push(mainBlock);
      }
      consume();
    }
    
    void parseBlockClose()
    {
      //if(mTokens[mPos+1].type == Type::KeywordElse)
      //  lastIfBlock = static_cast<ASTIfBlock*>(mainBlock);
      blocks.pop();
      mainBlock = blocks.top();
      consume();
    }
    
    void parseAssigment()
    {
      auto left = new ASTNode(consume()); //var
      auto root = new ASTNode(consume()); //assigment

      root->left = std::move(left);
      root->right = parseExpression(0);
      mainBlock->childs.push_back(root);
    }
    
public:

    Parser(const std::vector<Token> &t) : mTokens{t} {}

    ASTNode *parse()
    {
      if(mPos >= mTokens.size())
        return mainBlock; // main block
      
      if(peek().type == Type::KeywordElse) {
        parseElseKeyword();
      } else if(peek().type == Type::KeywordIf) {
        parseIfKeyword();
      } else if(peek().type == Type::BlockStart) {
        parseBlockOpen();
      } else if (peek().type == Type::BlockEnd && blocks.size() > 1) {
          parseBlockClose();
      } else if (peek().type == Type::Endl) {
        consume();
      } else if (peek().type == Type::Variable || peek().type == Type::Assigment) {
        parseAssigment();
      }
      return parse();
    }
};

#include <fstream>

int main()
{
  std::vector<Token> tokens;
  std::ifstream file("test.mlogpp");
    
  while(!file.eof()) {
    std::string txt;
    file >> txt;
//    std::cout<<txt<<std::endl;
    auto tmptokens = tokenize(txt);
    tokens.insert(tokens.end(), tmptokens.begin(), tmptokens.end());
    }
    for(const auto &token : tokens) {
        std::cout << token.value << ": " << token.typeName() << std::endl;
    }
    auto ast = Parser(tokens).parse();
    ast->outMlogCode(std::cout);
    return 0;
}
