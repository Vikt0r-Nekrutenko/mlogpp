#include "ast_node.hpp"

ASTNode::ASTNode(const Token &t) : token{t} {}

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
    if(token.type == Type::MultyString)
        return "\"" + token.value + "\"";
    if(token.type == Type::Number || token.type == Type::Variable)
        return token.value;
    if(token.type == Type::Assigment) {
        std::string value = rightNodeOutMlogCode(stream);
        stream << "set " << left->token.value << " " << value << std::endl;
        return left->token.value;
    }
    return "";
}

ASTOperatorNode::ASTOperatorNode(const Token &t) : ASTNode(t) {}

std::string ASTOperatorNode::outMlogCode(std::ostream &stream)
{
    std::string leftValue = leftNodeOutMlogCode(stream);
    std::string rightValue = rightNodeOutMlogCode(stream);
    std::string resultVariable = "_tempVar" + std::to_string(tempVariableN++);
    stream << token.getOpName() << " " << resultVariable << " " << leftValue << " " << rightValue << std::endl;
    return resultVariable;
}

ASTBlock::ASTBlock() : ASTNode{Token{"{", Type::BlockStart}} {}

std::string ASTBlock::outMlogCode(std::ostream &stream)
{
    for(auto ch : childs) {
        ch->outMlogCode(stream);
    }
    return "";
}

ASTIfBlock::ASTIfBlock() : ASTBlock()
{
    token.type = Type::KeywordIf;
    token.value = "if";
}

std::string ASTIfBlock::outMlogCode(std::ostream &stream)
{
    if(token.type == Type::KeywordIf) {
        std::string leftValue = leftNodeOutMlogCode(stream);
        stream << "jump " << label << " notEqual "  << leftValue << " true" << std::endl;

        ASTBlock::outMlogCode(stream);
        stream << label1 << ":" << std::endl;
    }
    return "";
}

ASTFunctionImplementationBlock::ASTFunctionImplementationBlock(const std::string &name) : ASTBlock()
{
    token.type = Type::FunctionImplementation;
    token.value = name;
}

ASTElseBlock::ASTElseBlock() : ASTBlock()
{
    token.type = Type::KeywordElse;
    token.value = "else";
}

std::string ASTElseBlock::outMlogCode(std::ostream &stream)
{
    if(token.type == Type::KeywordElse) {
        stream << label << ":" << std::endl;
        ASTBlock::outMlogCode(stream);
        stream << label1 << ":" << std::endl;
    }
    return "";
}

ASTMlogNode::ASTMlogNode(const std::string &v) : ASTNode({v, Type::KeywordMlog}) {}

std::string ASTMlogNode::outMlogCode(std::ostream &stream)
{
    stream << token.value << std::endl;
    return token.value;
}

ASTCellAccessNode::ASTCellAccessNode(const std::string &v, const std::string &av, CellAccessType t) : ASTNode({v, Type::Cell}), argumentValue{av},  accessType{t} {}

std::string ASTCellAccessNode::outMlogCode(std::ostream &stream)
{
    std::string rvalue = rightNodeOutMlogCode(stream);
    stream << (accessType == Read ? "read " : "write ") << argumentValue << " " << token.value << " " << rvalue << std::endl;
    return "";
}
