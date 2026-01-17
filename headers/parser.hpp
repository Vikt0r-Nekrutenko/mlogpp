#ifndef PARSER_HPP
#define PARSER_HPP

#include "ast_node.hpp"

using namespace mlogpp;

std::vector<Token> tokenize(const std::string &expression);

class Parser
{
    std::vector<Token> mTokens;
    std::stack<ASTNode *> blocks;
    ASTIfBlock *lastIfBlock = nullptr;
    ASTNode *mainBlock = nullptr;
    size_t mPos = 0;
    size_t mIfLblN = 0;

    Token peek();

    Token consume();

    ASTNode *parseExpression(int minPrec);

    ASTNode *parsePrimary();

    size_t findFunctionByName(const std::string &name);

    template <class BlockType>
    BlockType lastChildAsBlock()
    {
        return static_cast<BlockType>(mainBlock->childs.back());
    }

    template <class BlockType>
    ASTNode *addNewBlock()
    {
        mainBlock->childs.push_back(new BlockType(peek()));
        blocks.push(lastChildAsBlock<BlockType *>());
        return blocks.top();
    }

    template <class BlockType>
    ASTNode *addNewBlock(BlockType *block)
    {
        mainBlock->childs.push_back(block);
        blocks.push(lastChildAsBlock<BlockType *>());
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
