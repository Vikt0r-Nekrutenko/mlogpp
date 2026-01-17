#ifndef PARSER_HPP
#define PARSER_HPP

#include <regex>
#include "ast_node.hpp"

std::vector<Token> tokenize(const std::string &expression);

class Parser
{
    std::vector<Token> mTokens;
    std::stack<ASTBlock *> blocks;
    ASTIfBlock *lastIfBlock = nullptr;
    ASTBlock *mainBlock = nullptr;
    size_t mPos = 0;
    size_t mIfLblN = 0;

    Token peek();

    Token consume();

    ASTNode *parseExpression(int minPrec);

    ASTNode *parsePrimary();

    ASTBlock *lastChildAsBlock();

    size_t findFunctionByName(const std::string &name);

    template <class BlockType>
    ASTBlock *addNewBlock()
    {
        mainBlock->childs.push_back(new BlockType);
        blocks.push(lastChildAsBlock());
        return blocks.top();
    }

    template <class BlockType>
    ASTBlock *addNewBlock(BlockType *block)
    {
        mainBlock->childs.push_back(block);
        blocks.push(lastChildAsBlock());
        return blocks.top();
    }

    void parseIfKeyword();

    void parseElseKeyword();

    void parseBlockOpen();

    void parseBlockClose();

    void parseAssigment();

    void parseMlogKeyword();

    ASTCellAccessNode *parseCellAccess();

    void parseFunctionImplementation();

public:

    Parser(const std::vector<Token> &t);

    ASTNode *parse();
};

#endif // PARSER_HPP
