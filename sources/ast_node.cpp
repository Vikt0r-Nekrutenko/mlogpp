#include "ast_node.hpp"
#include <iostream>

using namespace mlogpp;
size_t ASTNode::tempVariableN = 0;

ASTNode::ASTNode(const Token &t) : token(Token(t.lineNumber(), t.value(), t.type())) {}

ASTNode::~ASTNode()
{
    delete left;
    delete right;
    for(size_t i = 0; i < childs.size(); ++i)
        delete childs[i];
}

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
    if(token.type() == mlogpp::Token::Type::MlogValue)
        stream << token.value() << std::endl;
    else if(token.type() == Token::Token::Type::String)
        return "\"" + token.value() + "\"";
    else if(token.type() == Token::Type::Number || token.type() == Token::Type::Variable || token.type() == Token::Type::Argument)
        return token.value();
    else if(token.type() == Token::Type::Assigment) {
        std::string value = rightNodeOutMlogCode(stream);
        stream << "set " << left->token.value() << " " << value << std::endl;
        return left->token.value();
    } else if(token.type() == Token::Type::BuildInFunctionCall) {
        if(token.value() == "sensor") {
            std::string resultVariable = "_tempVar" + std::to_string(tempVariableN++);
            stream << token.value() << " " << resultVariable << " " << childs.at(0)->token.value() << " "  << childs.at(1)->token.value() << std::endl;
            return resultVariable;
        } else if(token.value() == "control") {
            stream << token.value() << " " << childs.at(1)->token.value() << " "  << childs.at(0)->token.value() << " " << childs.at(2)->token.value() << std::endl;
        }
    }

    for(auto ch : childs) {
        ch->outMlogCode(stream);
    }
    return "";
}

void ASTNode::printTree(size_t depth) const
{
    if(left != nullptr) {
        left->printTree(depth);
    }

    std::cout << (left != nullptr ? " -> " : "") << "[" << token.value() << "]";
    if(token.type() == Token::Type::KeywordFunction) {
        auto ft = static_cast<const ASTFunctionImplementationBlock*>(this);
        for(auto p : ft->params)
            std::cout << p.value() << " ";
    }

    if(right != nullptr) {
        std::cout << " <- ";
        right->printTree(depth);
    }
    if(!childs.empty()){
        ++depth;
        std::cout << std::endl;
        for(auto child : childs) {
            for(auto d = depth; d > 0; --d)
                std::cout << "...";
            child->printTree(depth);
            std::cout << std::endl;
        }
        --depth;
    }
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

ASTIfBlock::ASTIfBlock(const Token &t)
    : ASTNode(t) {}

std::string ASTIfBlock::outMlogCode(std::ostream &stream)
{
    std::string leftValue = leftNodeOutMlogCode(stream);
    stream << "jump " << label << " notEqual "  << leftValue << " true" << std::endl;

    ASTNode::outMlogCode(stream);
    stream << label1 << ":" << std::endl;
    return "";
}

ASTFunctionImplementationBlock::ASTFunctionImplementationBlock(const Token &t)
    : ASTNode(t) {}

ASTElseBlock::ASTElseBlock(const Token &t)
    : ASTNode(t) {}

std::string ASTElseBlock::outMlogCode(std::ostream &stream)
{
    stream << label << ":" << std::endl;
    ASTNode::outMlogCode(stream);
    stream << label1 << ":" << std::endl;
    return "";
}

ASTCellAccessNode::ASTCellAccessNode(const Token &t, CellAccessType cat)
    : ASTNode(t), accessType{cat} {}

std::string ASTCellAccessNode::outMlogCode(std::ostream &stream)
{
    std::string lvalue = leftNodeOutMlogCode(stream);
    std::string rvalue = rightNodeOutMlogCode(stream);
    stream << (accessType == Read ? "read " : "write ") << lvalue << " " << token.value() << " " << rvalue << std::endl;
    return "";
}
