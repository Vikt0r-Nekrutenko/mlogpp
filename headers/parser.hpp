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
    std::stack<AST_Node *> blocks;
    AST_IfNode *lastIfBlock = nullptr;
    AST_Node *mainBlock = nullptr;
    size_t mPos = 0;
    size_t mIfLblN = 0;

    template <class BlockType> inline BlockType lastChildAsT() { return static_cast<BlockType>(mainBlock->childs.back()); }

    Token peek();

    Token consume();

    AST_Node *parseExpression(int minPrec);

    AST_Node *parsePrimary();

    size_t findFunctionByName(const std::string &name);

    AST_Node *addBlock(AST_Node *block);

    void parseIfKeyword();

    void parseElseKeyword();

    void parseBlockOpen();

    void parseBlockClose();

    AST_Node *parseAssigment();

    void parseMlogKeyword();

    AST_CellAccessNode *parseCellAccess();

    AST_Node *parseFunctionImplementation(bool callFromExpression = false);
    
    AST_Node *parseBuildInFunctionCall(bool callFromExpression = false);
    
    AST_Node *parseFunctionCall(bool callFromExpression = false);

public:

    Parser(const std::vector<Token> &t);

    AST_Node *parse();
};

#endif // PARSER_HPP
