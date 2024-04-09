#include <cstdio>
#include <cstdlib>

#include <iostream>

#include <catch2/catch_test_macros.hpp>

#include <cs/Lexer/Context.h>
#include <cs/Lexer/TokenUtil.h>

namespace lexer {

  enum MyTokens : unsigned {
    TOK_Identifier = cs::Token::make_userid(1),
    TOK_Integral,
    TOK_Real,
    TOK_String
  };

  using ctx = cs::LexerContext<char>;

  TEST_CASE("Scan identifiers.", "[identifier]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    ctx::Lexer lexer;
    lexer.addScanner(ctx::CIdentifierScanner::make(TOK_Identifier));
    lexer.initialize(" 0_0ident9 _ ");

    ctx::Token tok;

    tok = lexer.nextToken();
    REQUIRE( tok->id() == cs::TOK_Unknown );

    tok = lexer.nextToken();
    REQUIRE( tok->id() == TOK_Identifier );
    REQUIRE( cs::Token::to_value<std::string>(tok) == "_0ident9" );

    tok = lexer.nextToken();
    REQUIRE( tok->id() == TOK_Identifier );
    REQUIRE( cs::Token::to_value<std::string>(tok) == "_" );

    tok = lexer.nextToken();
    REQUIRE( tok->id() == cs::TOK_EndOfInput );
  }

  TEST_CASE("Scan integral values.", "[integral]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    ctx::Lexer lexer;
    lexer.addScanner(ctx::CIntegralScanner<unsigned>::make(TOK_Integral, true));
    lexer.initialize(" +0 123456789 01777 0xFf 0XfF ");

    ctx::Token tok;

    tok = lexer.nextToken();
    REQUIRE( tok->id() == cs::TOK_Unknown );

    tok = lexer.nextToken();
    REQUIRE( tok->id() == TOK_Integral );
    REQUIRE( cs::Token::to_value<unsigned>(tok) == unsigned{0} );

    tok = lexer.nextToken();
    REQUIRE( tok->id() == TOK_Integral );
    REQUIRE( cs::Token::to_value<unsigned>(tok) == unsigned{123456789} );

    tok = lexer.nextToken();
    REQUIRE( tok->id() == TOK_Integral );
    REQUIRE( cs::Token::to_value<unsigned>(tok) == unsigned{1023} );

    tok = lexer.nextToken();
    REQUIRE( tok->id() == TOK_Integral );
    REQUIRE( cs::Token::to_value<unsigned>(tok) == unsigned{255} );

    tok = lexer.nextToken();
    REQUIRE( tok->id() == TOK_Integral );
    REQUIRE( cs::Token::to_value<unsigned>(tok) == unsigned{255} );

    tok = lexer.nextToken();
    REQUIRE( tok->id() == cs::TOK_EndOfInput );
  }

  TEST_CASE("Scan real values.", "[real]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    constexpr auto   _0p25  =  0x0.4p0;
    constexpr auto   _1p0   =  0x1.0p0;
    constexpr auto   _1p25  =  0x1.4p0;
    constexpr auto  _25p0   = 0x19.0p0;
    constexpr auto _100p0   = 0x64.0p0;
    constexpr auto _125p0   = 0x7d.0p0;

    ctx::Lexer lexer;
    lexer.addScanner(ctx::CRealScanner<double>::make(TOK_Real));
    lexer.initialize(" eE1 125E-2 .25 1.25 1. .25e2 1.25e2 1.E2  ");

    ctx::Token tok;

    tok = lexer.nextToken();
    REQUIRE( tok->id() == cs::TOK_Unknown );
    REQUIRE( cs::Token::to_value<char>(tok) == 'e' );

    tok = lexer.nextToken();
    REQUIRE( tok->id() == cs::TOK_Unknown );
    REQUIRE( cs::Token::to_value<char>(tok) == 'E' );

    tok = lexer.nextToken();
    REQUIRE( tok->id() == cs::TOK_Unknown );
    REQUIRE( cs::Token::to_value<char>(tok) == '1' );

    tok = lexer.nextToken();
    REQUIRE( tok->id() == TOK_Real );
    REQUIRE( cs::Token::to_value<double>(tok) == _1p25 );

    tok = lexer.nextToken();
    REQUIRE( tok->id() == TOK_Real );
    REQUIRE( cs::Token::to_value<double>(tok) == _0p25 );

    tok = lexer.nextToken();
    REQUIRE( tok->id() == TOK_Real );
    REQUIRE( cs::Token::to_value<double>(tok) == _1p25 );

    tok = lexer.nextToken();
    REQUIRE( tok->id() == TOK_Real );
    REQUIRE( cs::Token::to_value<double>(tok) == _1p0 );

    tok = lexer.nextToken();
    REQUIRE( tok->id() == TOK_Real );
    REQUIRE( cs::Token::to_value<double>(tok) == _25p0 );

    tok = lexer.nextToken();
    REQUIRE( tok->id() == TOK_Real );
    REQUIRE( cs::Token::to_value<double>(tok) == _125p0 );

    tok = lexer.nextToken();
    REQUIRE( tok->id() == TOK_Real );
    REQUIRE( cs::Token::to_value<double>(tok) == _100p0 );

    tok = lexer.nextToken();
    REQUIRE( tok->id() == cs::TOK_EndOfInput );
  }

  TEST_CASE("Scan strings.", "[string]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    ctx::Lexer lexer;
    lexer.addScanner(ctx::CStringScanner::make(TOK_String));
    lexer.initialize(" \"Hello, World!\" \"\" ");

    ctx::Token tok;

    tok = lexer.nextToken();
    REQUIRE( tok->id() == TOK_String );
    REQUIRE( cs::Token::to_value<std::string>(tok) == "Hello, World!" );

    tok = lexer.nextToken();
    REQUIRE( tok->id() == TOK_String );
    REQUIRE( cs::Token::to_value<std::string>(tok).empty() );

    tok = lexer.nextToken();
    REQUIRE( tok->id() == cs::TOK_EndOfInput );
  }

  TEST_CASE("Skip whitespace.", "[whitespace]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    ctx::Lexer lexer;
    lexer.initialize(" \t\vX\n\f\r");

    ctx::Token tok;

    tok = lexer.nextToken();
    REQUIRE( tok->id() == cs::TOK_Unknown );
    REQUIRE( cs::Token::to_value<char>(tok) == 'X' );

    tok = lexer.nextToken();
    REQUIRE( tok->id() == cs::TOK_EndOfInput );
  }

} // namespace lexer
