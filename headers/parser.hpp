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

    inline ASTNode *lastChild() { return mainBlock->childs.back(); }

    template <class BlockType> inline BlockType lastChildAsT() { return static_cast<BlockType>(mainBlock->childs.back()); }

    Token peek();

    Token consume();

    ASTNode *parseExpression(int minPrec);

    ASTNode *parsePrimary();

    size_t findFunctionByName(const std::string &name);

    ASTNode *addBlock(ASTNode *block);

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
