#include <iostream>
#include <fstream>
#include <vector>

class Token
{
public:

    virtual std::string str() const { return ""; }

    virtual void print() const = 0;
};

class VariableToken : public Token
{
public:

    VariableToken(const std::string &name) : mVariableName{name} { }

    std::string str() const override { return mVariableName; }

    void print() const override { std::cout << "Var[" << mVariableName << "]" << std::endl; }

private:

    std::string mVariableName;
};

class EqualOperatorToken : public Token
{
public:

    std::string str() const override { return "="; }

    void print() const override { std::cout << "EqOp [" << "=" << "]" << std::endl; }
};

class AddOperatorToken : public Token
{
public:

    std::string str() const override { return "+"; }

    void print() const override { std::cout << "AddOp [" << "+" << "]" << std::endl; }
};

class NumberToken : public Token
{
public:

    NumberToken(const std::string &value) : mValue{value} { }

    std::string str() const override { return mValue; }

    void print() const override { std::cout << "Num[" << mValue << "]" << std::endl; }

private:

    std::string mValue;
};

void readFile(const std::string &fileName)
{
    std::string line;
    std::ifstream inputFile (fileName);
    std::vector<Token *> tokens;
    while ( getline (inputFile,line) )
    {
        std::cout << "Line: " << line << " size: " << line.size() << std::endl;
        std::string literal;
        size_t i = 0;
        while(i < line.size()) {
            while(line[i] != ' ' && line[i] != '\n' && line[i] != 0) {
                literal.push_back(line[i]);
                ++i;
            }

            try {
                std::stof(literal);
                tokens.push_back(new NumberToken(literal));
            } catch(...) {
                if(literal == "=") {
                    tokens.push_back(new EqualOperatorToken);
                } else if(literal == "+") {
                    tokens.push_back(new AddOperatorToken);
                } else {
                    tokens.push_back(new VariableToken(literal));
                }
            }
            std::cout << literal << std::endl;
            literal.clear();
            ++i;
        }
    }
    std::cout << "Tokens: " << std::endl;
    for(const auto &token : tokens) {
        token->print();
    }
    inputFile.close();
}

int main(int argc, char **argv)
{
    std::string fileName;
    if(argc == 2) {
        std::cout << argv[1] << std::endl;
        fileName = argv[1];
    }

    readFile(fileName);

    *(fileName.end() - 3) = 't';
    *(fileName.end() - 2) = 'x';
    *(fileName.end() - 1) = 't';

    // std::ofstream rfile(fileName);
    // rfile.close();
    return 0;
}
