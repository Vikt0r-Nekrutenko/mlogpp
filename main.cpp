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
    int mPos = 0;

    Token peek() { return mTokens[mPos]; }

    Token consume() { return mTokens[mPos++]; }

public:

    Parser(const std::vector<Token> &t) : mTokens{t} {}

    ASTNode *parse()
    {
        auto left = new ASTNode(NodeType::Variable, consume().value);
        consume();
        auto root = new ASTNode(NodeType::Assigment, "=");

        root->left = std::move(left);
        // root->right = parseExpression(0);

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
    return 0;
}
