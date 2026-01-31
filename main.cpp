// beta 2.1

#include "parser.hpp"
#include "syntax_error_handler.hpp"

#include <iostream>
#include <fstream>

void readFile(std::ifstream &sourceFile, std::ostream &resultFile) {
    mlogpp::SyntaxErrorHandler seh;
    std::vector<mlogpp::Token> tokens;
    size_t lineNumber = 1;

    try {
        while(!sourceFile.eof()) {
            std::string line;
            std::getline(sourceFile, line, '\n');
            mlogpp::tokenize(lineNumber++, tokens, line, seh);
        }
        seh.checkError(tokens, true);

        std::cout << "\n________Tokens:________" << std::endl;
        for(const auto &token : tokens) {
           std::cout << token.info() << std::endl;
        }
        std::cout << "\n________Parser info:________" << std::endl;
        auto ast = Parser(tokens).parse();
        std::cout << "\n________Program tree:________" << std::endl;
        ast->printTree(0);
        std::cout << "\n________Mlog code:________" << std::endl;
        ast->outMlogCode(resultFile);
    } catch(const std::string &ex) {
        std::cerr << "\tException: " << ex << std::endl;
    } catch(const char *ex) {
        std::cerr << "\ttException: " << ex << std::endl;
    }
}

int main(int argc, char **argv)
{
    std::ifstream sourceFile(argv[1]);
    std::ofstream outFile(argc > 2 ? argv[2] : "");
    std::ostream *resultFile = /*(argc > 2) ? &outFile :*/ &std::cout;

    readFile(sourceFile, *resultFile);
    return 0;
}

/*
function foo(str1, str2) {
  tmp = str1;
  str1 = str2;
  str2 = tmp;
  return str1 + str2;
}

  set a 4
  set b 5
  op lessThan _foo_result a b
  set f _foo_result
print f
printflush message1
end


*/
