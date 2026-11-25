////////////////////////////////////////////////////////////////////////////////

#include <Context.h>
#include <Lexer.h>

#include <print>

Context context;

int main(int argc, char** argv) {
  for (auto i = 1; i < argc; i++) {
    std::println("argument '{}'", argv[i]);

    Lexer l(argv[i]);
    std::string token;
    Lexer::Type type;
    while (l.token(token, type)) std::println("  token '{}' {}", token, Lexer::typeToString(type));
  }
}

////////////////////////////////////////////////////////////////////////////////
