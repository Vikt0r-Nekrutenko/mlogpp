#include "parser.hpp"
#include "ast_variable.hpp"
#include <iostream>

ASTFunctionImplementationBlock *currentFunction;

Token Parser::peek() { if(mPos >= mTokens.size()) throw "out of tokens range: " + std::to_string(mPos); return mTokens[mPos]; }

Token Parser::consume() { return mTokens[mPos++]; }

ASTNode *Parser::parseExpression(int minPrec)
{
    auto left = parsePrimary();
    while(mPos < mTokens.size() &&
           peek().type() != Token::Type::Assigment &&
           peek().type() != Token::Type::Endl &&
           peek().type() != Token::Type::BlockStart &&
           peek().type() != Token::Type::BlockEnd &&
           peek().type() != Token::Type::CellAccess &&
           mTokens[mPos].precedence() >= minPrec) {
        Token operatr = consume();
        if(operatr.value() == "]" || 
        operatr.value() == "=" || 
        operatr.value() == ")" || 
        operatr.type() == Token::Type::CellAccess) {
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
    if(peek().type() == Token::Type::BuildInFunctionCall) {
        return parseBuildInFunctionCall(true);
    }
    if(peek().type() == Token::Type::FunctionCall) {
        return parseFunctionCall(true);
    }
    if(peek().type() == Token::Type::CellAccess) {
        return parseCellAccess();
    }
    /*if(peek().type() == Token::Type::Assigment) {
        return parseAssigment();
    }*/
    Token t = consume();
    if(t.value() == "(" || t.value() == "[") {
        return parseExpression(0);
    }
    return new ASTNode(t);
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

ASTNode *Parser::addBlock(ASTNode *block)
{
    std::cout << "\tNow: [" << mainBlock->token.lineNumber() << ":" << mainBlock->token.value();
    mainBlock->childs.push_back(block);
    blocks.push(mainBlock->childs.back());
    std::cout << "] | Add: [" << blocks.top()->token.lineNumber() << ":" << blocks.top()->token.value() << "]" << std::endl;
    return blocks.top();
}

void Parser::parseIfKeyword()
{
    mainBlock = addBlock(new ASTIfBlock(peek()));
    consume();

    mainBlock->left = parseExpression(0);
    consume();

    lastIfBlock = static_cast<ASTIfBlock *>(mainBlock);
    lastIfBlock->label = lastIfBlock->label1 = std::string("ENDIF_") + std::to_string(++mIfLblN);
}

void Parser::parseElseKeyword()
{
    if(lastChildAsT<ASTIfBlock *>()->token.type() == Token::Type::KeywordIf){
        //lastIfBlock = lastChildAsT<ASTIfBlock *>();

        mainBlock = addBlock(new ASTElseBlock(peek()));

        static_cast<ASTElseBlock *>(mainBlock)->label1 = lastIfBlock->label;
        lastIfBlock->label1 = "jump " + lastIfBlock->label1 + " always";
        static_cast<ASTElseBlock *>(mainBlock)->label = lastIfBlock->label = std::string("ELSE_") + std::to_string(mIfLblN);
    }
    consume();
    consume();
}

void Parser::parseBlockOpen()
{
    if(mainBlock != nullptr) {
        mainBlock = addBlock(new ASTNode(peek()));
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
    std::cout << "\tDel: [" << mainBlock->token.lineNumber() << ":" << mainBlock->token.value();
    if(blocks.size() > 1)
        blocks.pop();
    mainBlock = blocks.top();
    std::cout << "] | Now: [" << mainBlock->token.lineNumber() << ":" << mainBlock->token.value() << "]" << std::endl;
    if(mainBlock->token.type() == Token::Type::FunctionName)
        currentFunction = (ASTFunctionImplementationBlock*)mainBlock;
    consume();
}

ASTNode *Parser::parseAssigment()
{
    auto tkn = peek().type() == Token::Type::Variable ? consume() : Token{peek().lineNumber(), "_", Token::Type::Variable};
    auto left = new AST_Variable(tkn); //var
    auto root = new ASTNode(consume()); //assigment

    root->left = std::move(left);
    root->right = parseExpression(0);
    if(tkn.value() == "_")
    mainBlock->childs.back()->left = root;
    else mainBlock->childs.push_back(root);
    return root;
}

void Parser::parseMlogKeyword()
{
    consume();
    auto mlog = new ASTNode(consume());
    mainBlock->childs.push_back(mlog);
}

ASTCellAccessNode *Parser::parseCellAccess()
{
    ASTCellAccessNode *root = nullptr;
    //if(mTokens.at(mPos - 1).type() == Token::Type::Assigment && mTokens.at(mPos - 2).type() == Token::Type::Variable)
    //{
        //auto argumentNode = new ASTNode(*mainBlock->childs.back()->left); // [x] = cell1[0]
        //delete mainBlock->childs.back(); // x [=] cell1[0]
        //mainBlock->childs.pop_back(); // x [=] cell1[0]

        root = new ASTCellAccessNode(peek(), ASTCellAccessNode::CellAccessType::Read);
        consume();
        consume(); // [
        size_t assigmentPos = mPos;
        while(++assigmentPos < mTokens.size() && mTokens[assigmentPos].value() != "]");
        if(mTokens.at(++assigmentPos).type() == Token::Type::Assigment) {
            root->accessType = ASTCellAccessNode::CellAccessType::Write;
            mainBlock->childs.push_back(root);
        }
        root->right = parseExpression(0);
        //root->left = argumentNode;
        
        //std::cerr<<"\t"<<peek().info()<<std::endl;
        return root;
    //}
    /*    auto argumentNode = new ASTNode(mTokens.at(assigmentPos)); // [x] = cell1[0]
        root = new ASTCellAccessNode(mTokens.at(mPos), ASTCellAccessNode::CellAccessType::Write);

        consume();
        root->left = argumentNode;
        root->right = parsePrimary();
        mainBlock->childs.push_back(root);
        mPos = assigmentPos + 1; // x = cell1[0][;]
    }*/
    //return root;
}

ASTNode *Parser::parseFunctionImplementation(bool callFromExpression)
{
    consume(); // pass 'function' keyword
    auto functionName = consume(); // and pass '('
    auto newBlock = new ASTFunctionImplementationBlock(functionName);
    currentFunction = newBlock;
    newBlock->label += functionName.value();
    consume();
    while(peek().value() != ")") {
        newBlock->params.push_back(consume());
    }
    
    consume(); // pass ')'
    consume(); // pass '{'
    if(mainBlock != nullptr) {
        //std::cerr<<"\tCurrent token 1: "<<newBlock->token.info()<<std::endl;
        if(!callFromExpression) {
            mainBlock = addBlock(newBlock);
        }
    } else {
        blocks.push(newBlock);
        //mainBlock = blocks.top();
        //std::cerr<<"\tCurrent token 2: "<<mainBlock->token.info()<<std::endl;
    }
    return newBlock;
}

ASTNode *Parser::parseBuildInFunctionCall(bool callFromExpression)
{
    auto node = new ASTNode(consume()); // pass function name
    consume(); // pass (
    while(peek().value() != ")") {
        
        auto argument = new ASTNode(peek());
        node->childs.push_back(argument);
        consume();
    }
    if(!callFromExpression)
        mainBlock->childs.push_back(node);
    consume();
    return node;
}

ASTNode *Parser::parseFunctionCall(bool callFromExpression)
{
    auto callPos = mPos;
    auto funcPos = findFunctionByName(peek().value());
    //auto node = new ASTNode(consume());
    consume(); // pass name
    consume(); // pass (
    std::vector<Token> arguments;
    while(peek().value() != ")") {
        arguments.push_back(consume());
    }
    mPos = funcPos;
    //std::cerr<<"\tCurrent token: "<<peek().info()<<std::endl;
    
    ASTFunctionImplementationBlock *fib = (ASTFunctionImplementationBlock*)parseFunctionImplementation(callFromExpression);
    //consume(); // pass 'function'
    //consume(); // pass function name
    //consume(); // pass (
    //for(auto a : arguments) std::cout<<"ARG:"<<a.value()<<std::endl;
    //std::cerr<<"\tCurrent token: "<<peek().info()<<std::endl;
    auto func = fib;
    for(size_t i = 0; i < arguments.size(); ++i) {
        auto argToParamAssigment = new ASTNode({0, "=", Token::Type::Assigment});
        argToParamAssigment->left = new ASTNode(func->params.at(i));
        argToParamAssigment->right = new ASTNode(arguments.at(i));
        func->childs.push_back(argToParamAssigment);
        //consume(); // jump to next argument
    }
    //func->printTree(0);
    //consume(); // pass )
    //consume(); // pass {
    // mainBlock = addBlock(node);
    parse();
    mPos = callPos; // return to call function
    while(peek().value() != ")")
        consume();
    consume(); // pass )
    return func;
}

Parser::Parser(const std::vector<Token> &t) : mTokens{t} {}

ASTNode *Parser::parse()
{
    if(mainBlock == nullptr && blocks.empty()) {
        mPos = findFunctionByName("main");
        mainBlock = parseFunctionImplementation();
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
    } else if (peek().type() == Token::Type::BlockEnd) {
        if(mainBlock->token.type() == Token::Type::FunctionName) {
            parseBlockClose();
            return nullptr;
        }
        parseBlockClose();
    } else if(peek().type() == Token::Type::KeywordMlog) {
        parseMlogKeyword();
    } else if (peek().type() == Token::Type::Endl) {
        consume();
    } else if (peek().type() == Token::Type::Variable || peek().type() == Token::Type::Assigment/* || peek().type() == Token::Type::Parameter || peek().type() == Token::Type::Argument*/) {
        parseAssigment();
    } else if (peek().type() == Token::Type::CellAccess) {
        parseCellAccess();
    } else if(peek().type() == Token::Type::KeywordFunction) {
        parseFunctionImplementation();
    } else if(peek().type() == Token::Type::BuildInFunctionCall) {
        parseBuildInFunctionCall();
    } else if(peek().type() == Token::Type::FunctionCall) {
        parseFunctionCall();
    } else if(peek().type() == Token::Type::Argument || peek().type() == Token::Type::Parameter) {
        //consume();
    } else if(peek().type() == Token::Type::ReturnKeyword) {
        auto node = new ASTReturnNode(consume());
        node->function = currentFunction;
        std::string retvarname = "_retVar_" + node->function->token.value();
        node->left = new AST_Variable({peek().lineNumber(), retvarname});
        node->right = parseExpression(0);
        currentFunction->childs.push_back(node);
    } 
    return parse();
}
