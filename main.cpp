#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <stack>
#include <queue>
#include <regex>

enum class Type {Variable, Number, Operator, Assigment, KeywordMlog, MultyString, BlockStart, BlockEnd, Endl, KeywordIf, KeywordElse};

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
    std::string resultVariable = "_tempVar" + std::to_string(tempVariableN++);
    stream << token.getOpName() << " " << resultVariable << " " << leftValue << " " << rightValue << std::endl;
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
      stream << "jump " << label << " notEqual "  << leftValue << " true" << std::endl;
      
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

struct ASTMlogNode : public ASTNode
{
  ASTMlogNode(const std::string &v) : ASTNode({v, Type::KeywordMlog}) {}
  
  std::string outMlogCode(std::ostream &stream) override
  {
    stream << token.value << std::endl;
    return token.value;
  }
};

bool waitStr = false;
std::string string;

std::vector<Token> tokenize(const std::string &expression)
{
    std::vector<Token> tokens;
    const std::regex pattern(
          R"(\s*("(?:[^"\\]|\\.)*")|)" // strings
          R"((and|or|if|else|mlog)|)" // keywords
          R"((-?\d+\.\d+|\d+)|)" // numbers
          R"(([a-zA-Z_][\w]*)|)" // variables
          R"((!=|==|<=|>=|[\;\+\-\/\*\=\(\)\<\>\&\|\%|\{|\}])|)" // operators
          );
    auto wordsBegin = std::sregex_iterator(expression.begin(), expression.end(), pattern);
    auto wordEnd = std::sregex_iterator();
    for(auto i = wordsBegin; i != wordEnd; ++i) {
        std::smatch match = *i;
        
        if(match[1].matched) { // strings
          std::string subStr = match[1].str();
          for(auto it = subStr.begin()+1; it != subStr.end()-1; ++it)
            if(*it != '\\')
              string += *it;
          string += '\n';
          if(expression.back() == ';') {
            string.pop_back(); // remove last '\n'
            waitStr = false;
            tokens.push_back({string, Type::MultyString});
            string.clear();
          }
        } else if(match[2].matched) { // keywords
          std::string keyword = match[2].str();
          if(keyword == "and" || keyword == "or") {
            tokens.push_back({keyword, Type::Operator});
          } else if(keyword == "if") {
            tokens.push_back({keyword, Type::KeywordIf});
          } else if(keyword == "else") {
            tokens.push_back({keyword, Type::KeywordElse});
          } else if(keyword == "mlog") {
            waitStr = true;
            tokens.push_back({keyword, Type::KeywordMlog});
          }
        } else if(match[3].matched) { // numbers
          tokens.push_back({match[3].str(), Type::Number});
        } else if(match[4].matched) { // variables
            tokens.push_back({match[4].str(), Type::Variable});
        } else if(match[5].matched) { // operators
          std::string buffer = match[5].str();
               if(buffer == "=")
            tokens.push_back({buffer, Type::Assigment});
          else if(buffer == "{")
            tokens.push_back({buffer, Type::BlockStart});
          else if(buffer == "}")
            tokens.push_back({buffer, Type::BlockEnd});
          else if(buffer == ";")
            tokens.push_back({buffer, Type::Endl});
          else
            tokens.push_back({buffer, Type::Operator});
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
    
    template <class BlockType>
    ASTBlock *addNewBlock()
    {
      mainBlock->childs.push_back(new BlockType);
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
    
    void parseMlogKeyword()
    {
      consume();
      auto mlog = new ASTMlogNode(consume().value);
      mainBlock->childs.push_back(mlog);
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
      } else if(peek().type == Type::KeywordMlog) {
        parseMlogKeyword();
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
    if(!waitStr)
      file >> txt;
    else {
      std::getline(file, txt, '\n');
    }
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
