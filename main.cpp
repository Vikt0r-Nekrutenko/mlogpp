#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <stack>
#include <queue>

enum class Type {Variable, Number, Operator, Assigment, BlockStart, BlockEnd, Endl, Keyword, KeywordIf};

const std::vector<std::string> Keywords {
  "and", "or", "if", "while",
};

struct Token
{
    std::string value;
    Type type;
    
    int precedence() const 
    { 
      if(value == "+" || value == "-")
        return 1;
      else if(value == "*" || value == "/")
        return 2;
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
        case Type::Keyword: return "Keyword";
        default: return "None";
      }
    }
    
    std::string getOpName() const
    {
             if(value == "+") return "add";
        else if(value == "-") return "sub";
        else if(value == "*") return "mul";
        else if(value == "/") return "div";
        return "";
    }
};

struct ASTNode
{
    Token token;
    ASTNode *left = nullptr;
    ASTNode *right = nullptr;

    ASTNode(const Token &t) : token{t} {}
};

struct ASTBlock : public ASTNode
{
  ASTBlock() : ASTNode{Token{"", Type::BlockStart}} {}
  std::vector<ASTNode*> childs;
};

struct ASTIfBlock : public ASTBlock
{
  ASTIfBlock() : ASTBlock() 
  {
    token.type = Type::KeywordIf;
    token.value = "if";
  }
  std::string label, label1;
};

Token tokenizeKeywords(const std::string &s)
{
  auto keywordIt = find_if(Keywords.begin(), Keywords.end(), [&](const std::string &keyword) {
    return s == keyword ? true : false;
  });
  if(keywordIt != Keywords.end()) {
    if(*keywordIt == "if")
      return {s, Type::KeywordIf};
    return {s, Type::Keyword};
  }
  return {s, Type::Variable};
}

std::vector<Token> tokenize(const std::string &expression)
{
    std::vector<Token> tokens;
    for(size_t i = 0; i < expression.length(); ++i) {
        if(std::isspace(expression[i]))
            continue;
        switch(expression[i]) {
        case '+':
        case '-':
        case '*':
        case '/':
        case '(':
        case ')':
            tokens.push_back({std::string(1, expression[i]), Type::Operator});
            break;
        case '=':
          tokens.push_back({std::string(1, expression[i]), Type::Assigment});
            break;
        case '{':
          tokens.push_back({std::string(1, expression[i]), Type::BlockStart});
          break;
        case '}':
          tokens.push_back({std::string(1, expression[i]), Type::BlockEnd});
          break;
        case ';':
          tokens.push_back({std::string(1, expression[i]), Type::Endl});
          break;
        }

        std::string buffer;
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
    std::string labelForNextBlock = "";
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
            auto node = new ASTNode(operatr);
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

public:

    Parser(const std::vector<Token> &t) : mTokens{t} {}

    ASTNode *parse()
    {
      if(mPos >= mTokens.size() - 1)
        return mainBlock; // main block
      
      if(peek().type == Type::KeywordIf) {
          mainBlock->childs.push_back(new ASTIfBlock);
          blocks.push(static_cast<ASTBlock*>(mainBlock->childs.back()));
          mainBlock = blocks.top();
          consume();
          
          mainBlock->left = parseExpression(0);
          consume();
          
          static_cast<ASTIfBlock*>(mainBlock)->label = static_cast<ASTIfBlock*>(mainBlock)->label1 = std::string("ENDIF_") + std::to_string(mIfLblN++);
      }
      
      if(peek().type == Type::BlockStart) {
        if(mainBlock != nullptr) {
          mainBlock->childs.push_back(new ASTBlock);
          blocks.push(static_cast<ASTBlock*>(mainBlock->childs.back()));
          mainBlock = blocks.top();
        } else {
          mainBlock = new ASTBlock;
          blocks.push(mainBlock);
        }
        consume();
      } else if (peek().type == Type::BlockEnd && blocks.size() > 1) {
          blocks.pop();
          mainBlock = blocks.top();
          consume();
      } else if (peek().type == Type::Endl) {
        consume();
      } else if (peek().type == Type::Variable || peek().type == Type::Assigment) {
        auto left = new ASTNode(consume()); //var
        auto root = new ASTNode(consume()); //assigment

        root->left = std::move(left);
        root->right = parseExpression(0);
        mainBlock->childs.push_back(root);
      }
      return parse();
    }
};

class Generator
{
    size_t mNReg = 0;

public:

    std::string generate(ASTNode *node)
    {
      if(node == nullptr) return  "";
        if(node->token.type== Type::Number || node->token.type== Type::Variable)
            return node->token.value;
        if(node->token.type== Type::Assigment) {
            std::string value = generate(node->right);
            std::cout << "set " << node->left->token.value << " " << value << std::endl;
            return node->left->token.value;
        }
        if(node->token.type== Type::Operator) {
            std::string leftValue = generate(node->left);
            std::string rightValue = generate(node->right);
            std::string resultVariable = "T" + std::to_string(mNReg++);
            std::cout << "op " << node->token.getOpName() << " " << resultVariable << " " << leftValue << " " << rightValue << std::endl;
            return resultVariable;
        }
        if(node->token.type == Type::KeywordIf) {
            std::string leftValue = generate(node->left);
            //std::string rightValue = generate(node->right);
            //std::string resultVariable = "T" + std::to_string(mNReg++);
            std::cout << "jump " << static_cast<ASTIfBlock*>(node)->label << " notEqual "  << leftValue << " true" << std::endl;
            //for(auto ch : static_cast<ASTBlock*>(node)->childs) {
            //  generate(ch);
            //}
            for(auto ch : static_cast<ASTIfBlock*>(node)->childs) {
              generate(ch);
            }
            std::cout << static_cast<ASTIfBlock*>(node)->label1 << ":" << std::endl;
        }
        if(node->token.type == Type::BlockStart) {
          for(auto ch : static_cast<ASTBlock*>(node)->childs) {
            generate(ch);
          }
        }
        return "";
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
      auto tmptokens = tokenize(txt);
      tokens.insert(tokens.end(), tmptokens.begin(), tmptokens.end());
    }
    /*for(const auto &token : tokens) {
        std::cout << token.value << " " << token.typeName() << std::endl;
    }*/
    try{
    auto ast = Parser(tokens).parse();
    Generator().generate(ast);
    }catch(const Token &t){
      std::cout<<"\t"<<t.value<<std::endl;
    }
    
    return 0;
}
