#include <cstdio>
#include <cstdlib>

#include <cs/Lexer/Lexer.h>
#include <cs/Lexer/Scanners/CString.h>
#include <cs/Lexer/TokenUtil.h>
#include <cs/Text/PrintUtil.h>

template<typename CharT>
struct LexerContext {
  using value_type = CharT;

  using Lexer = cs::Lexer<value_type>;
  using Token = cs::TokenPtr;

  using CStringScanner = cs::CStringScanner<value_type>;
};

enum MyTokens : cs::tokenid_t {
  TOK_String = cs::Token::make_userid(1)
};

int main(int argc, char **argv)
{
  using ctx = LexerContext<char>;

  ctx::Lexer lexer;
  lexer.addScanner(ctx::CStringScanner::make(TOK_String));
  lexer.initialize("  \n\n \"Hello, World!\"  ");

  for(ctx::Token tok = lexer.nextToken();
      tok->id() != cs::TOK_EndOfInput;
      tok = lexer.nextToken()) {
    if( !tok ) {
      cs::println("<nullptr>");
    } else if( tok->id() == cs::TOK_Unknown ) {
      cs::println("UNKNOWN(%,%): '%'",
                  tok->line(),
                  tok->column(),
                  cs::Token::to_value<char>(tok));
    } else if( tok->id() == TOK_String ) {
      cs::println("STRING(%,%): \"%\"",
                  tok->line(),
                  tok->column(),
                  cs::Token::to_value<std::string>(tok));
    }
  }

  return EXIT_SUCCESS;
}
