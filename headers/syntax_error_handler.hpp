#ifndef SYNTAX_ERROR_HANDLER_HPP
#define SYNTAX_ERROR_HANDLER_HPP

#include <vector>
#include <string>

namespace mlogpp {

class Token;

class BracketsChecker
{
public:

    void addOpenBracket();
    void addCloseBracket();

    int compare() const;

private:

    size_t mOpenBracketCounter {0};
    size_t mCloseBracketCounter {0};
};

class SyntaxErrorHandler
{
public:

    void checkError(const std::vector<Token> &tokens, bool isItFinalCheck=false);

private:

    std::string getUnexpectedTokenMessage(const Token &t) const;

    std::string getTooManyTokens(const Token &t) const;

    void bracketsCheck(BracketsChecker &checker, const Token &last, const std::string &ob, const std::string &cb, bool isItFinalCheck=false);

    bool findMainFunction(const std::vector<Token> &tokens) const;

    BracketsChecker mBlockBracketsChecker;
    BracketsChecker mArkBracketsChecker;
    BracketsChecker mSquareBracketsChecker;
};

}

#endif // SYNTAX_ERROR_HANDLER_HPP
