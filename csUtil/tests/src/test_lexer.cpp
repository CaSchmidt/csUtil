#include <cstdio>
#include <cstdlib>

#include <print>

#include <cs/Lexer/Context.h>
#include <cs/Lexer/TokenUtil.h>
#include <cs/Math/Numeric.h>
#include <cs/Text/StringUtil.h>

enum MyTokens : cs::tokenid_t {
  TOK_Identifier = cs::Token::make_userid(1),
  TOK_String,
  TOK_UInt
};

void printText(const std::string_view& text,
               const std::string_view& title = std::string_view())
{
  const std::list<std::string> lines = cs::split(text, '\n');
  if( lines.empty() ) {
    return;
  }

  const std::size_t width = cs::countDigits(lines.size());

  if( !title.empty() ) {
    std::println("{} =", title);
  }

  std::size_t lineno = 1;
  for( const std::string& line : lines ) {
    std::println("{0:{1}}: >{2}<", lineno++, width, line);
  }

  if( !title.empty() ) {
    std::println("");
  }
}

int main(int argc, char **argv)
{
  using ctx = cs::LexerContext<char>;

  const char *input =
      "  \n"
      "\n"
      " \"Hello, World!\";  \"\",\n"
      " 0_0ident9. _:\n"
      "+0, 123456789, 01777, 0xFf, 0XfF";

  printText(input, "input");

  ctx::Lexer lexer;
  lexer.addScanner(ctx::CIdentifierScanner::make(TOK_Identifier));
  lexer.addScanner(ctx::CStringScanner::make(TOK_String));
  lexer.addScanner(ctx::CIntegralScanner<unsigned>::make(TOK_UInt, true));
  lexer.initialize(input);

  constexpr bool DISP_UNKNOWN = false;

  for(cs::TokenPtr tok = lexer.nextToken();
      tok->id() != cs::TOK_EndOfInput;
      tok = lexer.nextToken()) {
    if( !tok ) {
      std::println("<nullptr>");

    } else if( tok->id() == cs::TOK_Unknown  &&  DISP_UNKNOWN ) {
      std::println("UNKNOWN({},{}): '{}'",
                   tok->line(),
                   tok->column(),
                   cs::Token::to_value<char>(tok));

    } else if( tok->id() == TOK_Identifier ) {
      std::println("IDENTIFIER({},{}): {}",
                   tok->line(),
                   tok->column(),
                   cs::Token::to_value<std::string>(tok));

    } else if( tok->id() == TOK_String ) {
      std::println("STRING({},{}): \"{}\"",
                   tok->line(),
                   tok->column(),
                   cs::Token::to_value<std::string>(tok));

    } else if( tok->id() == TOK_UInt ) {
      std::println("INTEGRAL({},{}): {}",
                   tok->line(),
                   tok->column(),
                   cs::Token::to_value<unsigned>(tok));
    }
  } // For each token

  return EXIT_SUCCESS;
}
