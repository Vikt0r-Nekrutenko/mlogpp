#include <iostream>
#include <memory>
#include <string>
#include <vector>

enum class Type {Variable, Number, Operator, Assigment};

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
        }

        std::string buffer;
        while(i < expression.length() && (std::isalnum(expression[i]) || expression[i] == '_')) {
            buffer += expression[i++];
        }
        if(!buffer.empty()) {
          if(std::isdigit(buffer[0])) {
            tokens.push_back({buffer, Type::Number});
          } else {
            tokens.push_back({buffer, Type::Variable});
          }
          i--;
        }
    }
    return tokens;
}

class Parser
{
    std::vector<Token> mTokens;
    size_t mPos = 0;

    Token peek() { return mTokens[mPos]; }

    Token consume() { return mTokens[mPos++]; }

    int precedence(const std::string &op)
    {
        if(op == "+" || op == "-")
            return 1;
        else if(op == "*" || op == "/")
            return 2;
        return 0;
    }

    ASTNode *parseExpression(int minPrec)
    {
        auto left = parsePrimary();
        while(mPos < mTokens.size() && mTokens[mPos].precedence() >= minPrec) {
            Token operatr = consume();
            if(operatr.value == ")"){
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
        auto left = new ASTNode(consume()); //var
        auto root = new ASTNode(consume()); //assigment

        root->left = std::move(left);
        root->right = parseExpression(0);

        return root;
    }
};

class Generator
{
    size_t mNReg = 0;

public:

    std::string generate(ASTNode *node)
    {
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
        return "";
    }
};

int main()
{
    std::string line = "a = (2 - 2) * (6-4) b = 8 * 4";
    auto tokens = tokenize(line);
    for(const auto &token : tokens) {
        std::cout << token.value << " " << token.typeName() << std::endl;
    }
    auto ast = Parser(tokens).parse();
    Generator().generate(ast);
    return 0;
}
