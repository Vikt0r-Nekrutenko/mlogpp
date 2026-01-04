#include <iostream>
#include <regex>
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

    virtual std::string getMlogValue() const { return value; }
};

struct ASTNodeOperator : public ASTNode
{
    ASTNodeOperator(const std::string &s) : ASTNode(NodeType::Operator, s) {}

    std::string getMlogValue() const override
    {
        if(value == "+") return "op add";
        else if(value == "-") return "op sub";
        else if(value == "*") return "op mul";
        else if(value == "/") return "op div";
        return value;
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
    const std::regex pattern(R"(([\d]+)|([a-zA-Z_][\w]*)|(!=|==|<=|>=|&&|\|\||[\+\-\/\*\=\(\)\<\>\&\|\%]))");
    auto wordsBegin = std::sregex_iterator(expression.begin(), expression.end(), pattern);
    auto wordEnd = std::sregex_iterator();
    for(auto i = wordsBegin; i != wordEnd; ++i) {
        std::smatch match = *i;
        std::string value = match.str();

        tokens.push_back({value, match[3].matched});
    }
    return tokens;
}

class Parser
{
    std::vector<Token> mTokens;
    size_t mPos = 0;

    Token peek() { return mPos < mTokens.size() ? mTokens[mPos] : Token{"", false}; }

    Token consume() { return mTokens[mPos++]; }

    int precedence(const std::string &op)
    {
        if(op == "*" || op == "/" || op == "%")
            return 6;
        else if(op == "+" || op == "-")
            return 5;
        else if(op == "<" || op == ">" || op == "<=" || op == ">=")
            return 4;
        else if(op == "==" || op == "!=")
            return 3;
        else if(op == "&" || op == "|")
            return 2;
        else if(op == "&&" || op == "||")
            return 1;
        return 0;
    }

    ASTNode *parseExpression(int minPrec)
    {
        auto left = parsePrimary();
        while(mPos < mTokens.size()){
            Token op = peek();
            if(op.value == ")" || precedence(op.value) < minPrec)
                break;
            consume();
            auto node = new ASTNodeOperator(op.value);
            node->left = std::move(left);
            node->right = parseExpression(precedence(op.value) + 1);
            left = std::move(node);
        }
        return left;
    }

    ASTNode *parsePrimary()
    {
        Token t = consume();
        if(t.value == "(") {
            auto node = parseExpression(0);
            consume();
            return node;
        }
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

class Generator
{
    size_t mNReg = 0;

public:

    std::string generate(ASTNode *node)
    {
        if(node == nullptr) return "";
        if(node->type == NodeType::Number || node->type == NodeType::Variable)
            return node->value;
        if(node->type == NodeType::Assigment) {
            std::string value = generate(node->right);
            std::cout << "set " << node->left->value << " " << value << std::endl;
            return node->left->value;
        }
        if(node->type == NodeType::Operator) {
            std::string leftValue = generate(node->left);
            std::string rightValue = generate(node->right);
            std::string resultVariable = "T" + std::to_string(mNReg++);
            std::cout << node->getMlogValue() << " " << resultVariable << " " << leftValue << " " << rightValue << std::endl;
            return resultVariable;
        }
        return "";
    }
};

int main()
{
    std::string line = "a = (5 + a_b) == 2<=54&1";
    auto tokens = tokenize(line);
    for(const auto &token : tokens) {
        // std::cout << token.value << " " << token.isOperator << std::endl;
        std::cout << token.value << "_";
    } std::cout << std::endl;
    // auto ast = Parser(tokens).parse();
    // Generator().generate(ast);

    return 0;
}
/**
 * op lessThan _reg1 a b
 * op greaterThan _reg1 a b
 * op lessThanEq _reg1 a b
 * op greaterThanEq _reg1 a b
 * op strictEqual _reg1 a b

op strictEqual _reg1 1 2
jump ELSE1 notEqual _reg1 true
op add a 2 2
jump ENDIF1 always
ELSE1:
op add a 1 1
ENDIF1:
set b 0

if(1 == 2) {
    a = 2 + 2
} else {
    a = 1 + 1
}
*/
