#include <cstdio>
#include <cstdlib>

#include <cs/Lexer/Context.h>
#include <cs/Lexer/TokenUtil.h>
#include <cs/Text/PrintUtil.h>

enum MyTokens : cs::tokenid_t {
  TOK_Identifier = cs::Token::make_userid(1),
  TOK_String
};

int main(int argc, char **argv)
{
  using ctx = cs::LexerContext<char>;

  const char *text =
      "  \n"                    // Line #0
      "\n"                      // Line #1
      " \"Hello, World!\";  \n" // Line #2
      " 0_0ident9. _:";         // Line #3

  ctx::Lexer lexer;
  lexer.addScanner(ctx::CIdentifierScanner::make(TOK_Identifier));
  lexer.addScanner(ctx::CStringScanner::make(TOK_String));
  lexer.initialize(text);

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
    } else if( tok->id() == TOK_Identifier ) {
      cs::println("IDENTIFIER(%,%): %",
                  tok->line(),
                  tok->column(),
                  cs::Token::to_value<std::string>(tok));
    } else if( tok->id() == TOK_String ) {
      cs::println("STRING(%,%): \"%\"",
                  tok->line(),
                  tok->column(),
                  cs::Token::to_value<std::string>(tok));
    }
  } // For each token

  return EXIT_SUCCESS;
}
