#include "ast_node.hpp"
#include <iostream>

using namespace mlogpp;
size_t ASTNode::tempVariableN = 0;

ASTNode::ASTNode(const Token &t) : token(Token(t.lineNumber(), t.value(), t.type())) {}

std::string ASTNode::leftNodeOutMlogCode(std::ostream &stream)
{
    if(left == nullptr)
        return "";
    return left->outMlogCode(stream);
}

std::string ASTNode::rightNodeOutMlogCode(std::ostream &stream)
{
    if(right == nullptr)
        return "";
    return right->outMlogCode(stream);
}

std::string ASTNode::outMlogCode(std::ostream &stream)
{
    if(token.type() == Token::Token::Type::String)
        return "\"" + token.value() + "\"";
    if(token.type() == Token::Type::Number || token.type() == Token::Type::Variable)
        return token.value();
    if(token.type() == Token::Type::Assigment) {
        std::string value = rightNodeOutMlogCode(stream);
        stream << "set " << left->token.value() << " " << value << std::endl;
        return left->token.value();
    }
    return "";
}

ASTOperatorNode::ASTOperatorNode(const Token &t)
    : ASTNode(t) {}

std::string ASTOperatorNode::outMlogCode(std::ostream &stream)
{
    std::string leftValue = leftNodeOutMlogCode(stream);
    std::string rightValue = rightNodeOutMlogCode(stream);
    std::string resultVariable = "_tempVar" + std::to_string(tempVariableN++);
    stream << token.getOpName() << " " << resultVariable << " " << leftValue << " " << rightValue << std::endl;
    return resultVariable;
}

ASTBlock::ASTBlock(const Token &t)
    : ASTNode(t) {}

std::string ASTBlock::outMlogCode(std::ostream &stream)
{
    for(auto ch : childs) {
        ch->outMlogCode(stream);
    }
    return "";
}

ASTIfBlock::ASTIfBlock(const Token &t)
    : ASTBlock(t)
{
    token.type() = Token::Type::KeywordIf;
    token.value() = "if";
}

std::string ASTIfBlock::outMlogCode(std::ostream &stream)
{
    if(token.type() == Token::Token::Type::KeywordIf) {
        std::string leftValue = leftNodeOutMlogCode(stream);
        stream << "jump " << label << " notEqual "  << leftValue << " true" << std::endl;

        ASTBlock::outMlogCode(stream);
        stream << label1 << ":" << std::endl;
    }
    return "";
}

ASTFunctionImplementationBlock::ASTFunctionImplementationBlock(const Token &t)
    : ASTBlock(t) {}

ASTElseBlock::ASTElseBlock(const Token &t)
    : ASTBlock(t) {}

std::string ASTElseBlock::outMlogCode(std::ostream &stream)
{
    if(token.type() == Token::Type::KeywordElse) {
        stream << label << ":" << std::endl;
        ASTBlock::outMlogCode(stream);
        stream << label1 << ":" << std::endl;
    }
    return "";
}

ASTMlogNode::ASTMlogNode(const Token &t)
    : ASTNode(t) {}

std::string ASTMlogNode::outMlogCode(std::ostream &stream)
{
    stream << token.value() << std::endl;
    return token.value();
}

ASTCellAccessNode::ASTCellAccessNode(const Token &t, const std::string &av, CellAccessType cat)
    : ASTNode(t), argumentValue{av},  accessType{cat} {}

std::string ASTCellAccessNode::outMlogCode(std::ostream &stream)
{
    std::string rvalue = rightNodeOutMlogCode(stream);
    stream << (accessType == Read ? "read " : "write ") << argumentValue << " " << token.value() << " " << rvalue << std::endl;
    return "";
}
