#include <iostream>
#include <string>
#include <vector>

enum class NodeType {Variable, Number, Operator, Assigment};

struct ASTNode
{
    NodeType type;
    std::string value;
    ASTNode *left = nullptr;
    ASTNode *right = nullptr;

    ASTNode(NodeType t, const std::string &s) : type{t}, value{s} {}

    void print()
    {
        std::cout << value << " L: ";
        if(left != nullptr) left->print();
        std::cout << " R: ";
        if(right != nullptr) right->print();
        std::cout << std::endl;
    }
};

struct Token
{
    std::string value;
    bool isOperator;
};

std::vector<Token> tokenize(const std::string &expression)
{
    std::vector<Token> tokens;
    std::string buffer;
    for(size_t i = 0; i < expression.length(); ++i) {
        if(std::isspace(expression[i]))
            continue;
        if(std::ispunct(expression[i]) && expression[i] != '_') {
            if(!buffer.empty()) {
                tokens.push_back({buffer, false});
                buffer.clear();
            }
            tokens.push_back({std::string(1 , expression[i]), true});
        } else {
            buffer += expression[i];
        }
    }
    if(!buffer.empty()) {
        tokens.push_back({buffer, false});
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
        while(mPos < mTokens.size() && precedence(mTokens[mPos].value) >= minPrec) {
            Token op = consume();
            auto node = new ASTNode(NodeType::Operator, op.value);
            node->left = std::move(left);
            node->right = parseExpression(precedence(op.value) + 1);
            left = std::move(node);
        }
        return left;
    }

    ASTNode *parsePrimary()
    {
        Token t = consume();
        if(std::isdigit(t.value[0]))
            return new ASTNode(NodeType::Number, t.value);
        return new ASTNode(NodeType::Variable, t.value);
    }

public:

    Parser(const std::vector<Token> &t) : mTokens{t} {}

    ASTNode *parse()
    {
        auto left = new ASTNode(NodeType::Variable, consume().value);
        consume();
        auto root = new ASTNode(NodeType::Assigment, "=");

        root->left = std::move(left);
        root->right = parseExpression(0);

        return root;
    }
};

int main()
{
    std::string line = "a = 5 + 2 * 2";
    auto tokens = tokenize(line);
    for(const auto &token : tokens) {
        std::cout << token.value << " " << token.isOperator << std::endl;
    }
    auto ast = Parser(tokens).parse();
    ast->print();
    return 0;
}
