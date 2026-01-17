#include "syntax_error_handler.hpp"
#include "tokenizer.hpp"

void mlogpp::BracketsChecker::addOpenBracket() { ++mOpenBracketCounter; }

void mlogpp::BracketsChecker::addCloseBracket() { ++mCloseBracketCounter; }

int mlogpp::BracketsChecker::compare() const
{
    if(mOpenBracketCounter > mCloseBracketCounter)
        return +1;
    if(mOpenBracketCounter < mCloseBracketCounter)
        return -1;
    return 0;
}

void mlogpp::SyntaxErrorHandler::checkError(const std::vector<Token> &tokens, bool isItFinalCheck)
{
    Token last = tokens.back();
    if(last.value == "{" || last.value == "}") {
        bracketsCheck(mBlockBracketsChecker, last, "{", "}", isItFinalCheck);
    } else if(last.value == "(" || last.value == ")") {
        bracketsCheck(mArkBracketsChecker, last, "(", ")", isItFinalCheck);
    } else if(last.value == "[" || last.value == "]") {
        bracketsCheck(mSquareBracketsChecker, last, "[", "]", isItFinalCheck);
    }
}

std::string mlogpp::SyntaxErrorHandler::getUnexpectedTokenMessage(const Token &t) const
{
    return std::to_string(t.line_number) + std::string(" | SyntaxErrorHandler: Unexpected token - [") + t.value + "]";
}

std::string mlogpp::SyntaxErrorHandler::getTooManyTokens(const Token &t) const
{
    return std::string(" | SyntaxErrorHandler: Too many tokens - [") + t.value + "]";
}

void mlogpp::SyntaxErrorHandler::bracketsCheck(BracketsChecker &checker, const Token &last, const std::string &ob, const std::string &cb, bool isItFinalCheck)
{
    if(last.value == ob)
        checker.addOpenBracket();
    if(last.value == cb) {
        if(!isItFinalCheck)
            checker.addCloseBracket();
        if(!isItFinalCheck && checker.compare() == -1)
            throw getUnexpectedTokenMessage(last);
        if(isItFinalCheck && checker.compare() == +1)
            throw getTooManyTokens(Token(last.line_number, ob, Token::Type::Operator));
    }
}
