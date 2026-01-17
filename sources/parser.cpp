#include "parser.hpp"

Token Parser::peek() { return mTokens[mPos]; }

Token Parser::consume() { return mTokens[mPos++]; }

ASTNode *Parser::parseExpression(int minPrec)
{
    auto left = parsePrimary();
    while(mPos < mTokens.size() &&
           peek().type() != Token::Type::Endl &&
           peek().type() != Token::Type::BlockStart &&
           peek().type() != Token::Type::BlockEnd &&
           peek().type() != Token::Type::CellAccess &&
           mTokens[mPos].precedence() >= minPrec) {
        Token operatr = consume();
        if(operatr.value() == ")" || operatr.value() == "]" || operatr.type() == Token::Type::CellAccess) {
            break;
        }
        auto node = new ASTOperatorNode(operatr);
        node->left = std::move(left);
        node->right = parseExpression(operatr.precedence() + 1);
        left = std::move(node);
    }
    return left;
}

ASTNode *Parser::parsePrimary()
{
    Token t = consume();
    if(t.value() == "(" || t.value() == "["){
        return parseExpression(0);
    } else if(t.type() == Token::Type::CellAccess) {
        mPos-=1;
        return nullptr;
    }
    return new ASTNode(t);
}

ASTNode *Parser::lastChildAsBlock()
{
    return static_cast<ASTNode*>(mainBlock->childs.back());
}

size_t Parser::findFunctionByName(const std::string &name)
{
    for(size_t i = 0; i < mTokens.size(); ++i) {
        if(mTokens[i+1].value() == name && mTokens[i].type() == Token::Type::KeywordFunction) {
            return i;
        }
    }
    return mPos;
}

void Parser::parseIfKeyword()
{
    mainBlock = addNewBlock<ASTIfBlock>();
    consume();

    mainBlock->left = parseExpression(0);
    consume();

    static_cast<ASTIfBlock*>(mainBlock)->label = static_cast<ASTIfBlock*>(mainBlock)->label1 = std::string("ENDIF_") + std::to_string(++mIfLblN);
}

void Parser::parseElseKeyword()
{
    if(lastChildAsBlock()->token.type() == Token::Type::KeywordIf){
        lastIfBlock = static_cast<ASTIfBlock*>(lastChildAsBlock());

        mainBlock = addNewBlock<ASTElseBlock>();

        static_cast<ASTElseBlock*>(mainBlock)->label1 = lastIfBlock->label;
        lastIfBlock->label1 = "jump " + lastIfBlock->label1 + " always";
        static_cast<ASTElseBlock*>(mainBlock)->label = lastIfBlock->label = std::string("ELSE_") + std::to_string(mIfLblN);
    }
    consume();
}

void Parser::parseBlockOpen()
{
    if(mainBlock != nullptr) {
        mainBlock = addNewBlock<ASTNode>();
    } else {
        mainBlock = new ASTNode(peek());
        blocks.push(mainBlock);
    }
    consume();
}

void Parser::parseBlockClose()
{
    //if(mTokens[mPos+1].type() == Token::Type::KeywordElse)
    //  lastIfBlock = static_cast<ASTIfBlock*>(mainBlock);
    if(blocks.size() > 1)
        blocks.pop();
    mainBlock = blocks.top();
    consume();
}

void Parser::parseAssigment()
{
    auto left = new ASTNode(consume()); //var
    auto root = new ASTNode(consume()); //assigment

    root->left = std::move(left);
    root->right = parseExpression(0);
    mainBlock->childs.push_back(root);
}

void Parser::parseMlogKeyword()
{
    consume();
    auto mlog = new ASTMlogNode(consume());
    mainBlock->childs.push_back(mlog);
}

ASTCellAccessNode *Parser::parseCellAccess()
{
    ASTCellAccessNode *root = nullptr;
    if(mPos - 2 >= 0 && mTokens.at(mPos - 1).type() == Token::Type::Assigment && mTokens.at(mPos - 2).type() == Token::Type::Variable)
    {
        delete mainBlock->childs.back();
        mainBlock->childs.pop_back();
        root = new ASTCellAccessNode(mTokens.at(mPos), mTokens.at(mPos-2).value(), ASTCellAccessNode::CellAccessType::Read);
        consume();
        root->right = parsePrimary();
        mainBlock->childs.push_back(root);
        return root;
    }
    size_t assigmentPos = mPos;
    while(++assigmentPos < mTokens.size() && mTokens[assigmentPos].type() != Token::Type::Assigment);
    if(mTokens.at(++assigmentPos).type() == Token::Type::Variable) {
        root = new ASTCellAccessNode(mTokens.at(mPos), mTokens.at(assigmentPos).value(), ASTCellAccessNode::CellAccessType::Write);

        consume();
        root->right = parsePrimary();
        mainBlock->childs.push_back(root);
        mPos = assigmentPos + 1;
    }
    return root;
}

void Parser::parseFunctionImplementation()
{
    consume();
    auto functionName = consume();
    consume();
    consume();
    //      std::cerr<<"\t"<<peek().value()<<std::endl;throw;
    if(mainBlock != nullptr) {
        mainBlock = addNewBlock<ASTFunctionImplementationBlock>(new ASTFunctionImplementationBlock(functionName));
    } else {
        mainBlock = new ASTFunctionImplementationBlock(functionName);
        blocks.push(mainBlock);
    }
    consume();
}

Parser::Parser(const std::vector<Token> &t) : mTokens{t} {}

ASTNode *Parser::parse()
{
    if(mainBlock == nullptr && blocks.empty()) {
        mPos = findFunctionByName("main");
        parseFunctionImplementation();
        if(mainBlock == nullptr && blocks.empty())
            throw "'function main()' does not exist!";
    }
    if(mPos >= mTokens.size() - 1) {
        return mainBlock; // main block
    } else if(peek().type() == Token::Type::KeywordElse) {
        parseElseKeyword();
    } else if(peek().type() == Token::Type::KeywordIf) {
        parseIfKeyword();
    } else if(peek().type() == Token::Type::BlockStart) {
        parseBlockOpen();
    } else if (peek().type() == Token::Type::BlockEnd && blocks.size() > 1) {
        parseBlockClose();
    } else if(peek().type() == Token::Type::KeywordMlog) {
        parseMlogKeyword();
    } else if (peek().type() == Token::Type::Endl) {
        consume();
    } else if (peek().type() == Token::Type::Variable || peek().type() == Token::Type::Assigment) {
        parseAssigment();
    } else if (peek().type() == Token::Type::CellAccess) {
        parseCellAccess();
    } else if(peek().type() == Token::Type::KeywordFunction) {
        parseFunctionImplementation();
    }
    return parse();
}
