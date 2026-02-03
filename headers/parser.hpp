#ifndef PARSER_HPP
#define PARSER_HPP

#include <stack>
#include "ast_node.hpp"
#include "ast_cell_access_node.hpp"
#include "ast_if_node.hpp"

using namespace mlogpp;

class Parser
{
    std::vector<Token> mTokens;
    std::stack<ASTNode *> blocks;
    ASTIfBlock *lastIfBlock = nullptr;
    ASTNode *mainBlock = nullptr;
    size_t mPos = 0;
    size_t mIfLblN = 0;

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

    ASTNode *parseAssigment();

    void parseMlogKeyword();

    AST_CellAccessNode *parseCellAccess();

    ASTNode *parseFunctionImplementation(bool callFromExpression = false);
    
    ASTNode *parseBuildInFunctionCall(bool callFromExpression = false);
    
    ASTNode *parseFunctionCall(bool callFromExpression = false);

public:

    Parser(const std::vector<Token> &t);

    ASTNode *parse();
};

#endif // PARSER_HPP
