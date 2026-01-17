// beta 2.1

#include "tokenizer.hpp"
#include "syntax_error_handler.hpp"

#include <iostream>
#include <fstream>

int main(int argc, char **argv)
{
  mlogpp::SyntaxErrorHandler seh;
  std::vector<mlogpp::Token> tokens;
  std::ifstream file(argv[1]);
  size_t lineNumber = 1;
    
  try {
    while(!file.eof()) {
      std::string line;
      std::getline(file, line, '\n');
      mlogpp::tokenize(lineNumber++, tokens, line, seh);
      seh.checkError(tokens, true);
    }
    for(const auto &token : tokens) {
      std::cout << token.info() << std::endl;
    }
  } catch(const std::string &ex) {
    std::cerr << "\t" << ex << std::endl;
  } catch(const char *ex) {
      std::cerr << "\t" << ex << std::endl;
  }

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
