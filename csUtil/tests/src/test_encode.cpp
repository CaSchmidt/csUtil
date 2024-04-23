#include <cstdio>
#include <cstdlib>
#include <cstddef>

#include <charconv>

#include <cs/Lexer/Context.h>
#include <cs/Lexer/Parser.h>
#include <cs/Text/PrintFormat.h>
#include <cs/Text/PrintUtil.h>

#include "Encode/Encode.h"

template<typename T>
inline T stringToUInt(const char *first, const char *last,
                      const int base = 10, const T defValue = 0)
{
  T value = defValue;
  const std::from_chars_result result =
      std::from_chars(first, last, value, base);

  return result.ec == std::errc()
      ? value
      : defValue;
}

template<typename T>
void inputVariables(Encode::VariableStore<T> *store)
{
  using citer_t = typename Encode::VariableStore<T>::const_iterator;

  for(citer_t it = store->cbegin(); it != store->end(); ++it) {
    cs::print("% (== 0x%) := ", it->first, cs::hexf(it->second));

    std::string input;
    std::cin >> input;
    if( input.empty() ) {
      continue;
    }
    cs::lower(input);

    const T value = cs::startsWith(input, "0x", true)
        ? stringToUInt<T>(input.data() + 2, input.data() + input.size(), 16)
        : stringToUInt<T>(input.data(), input.data() + input.size());

    (*store)[it->first] = value;
  }
}

/*
 * Proposed Syntax:
 *
 * encode32("description") = { FIELD , FIELD, ... }
 *
 * With:
 *
 * FIELD = (IDENTIFIER | LITERAL) [to:from] @ at
 */
void test_encode32()
{
  using ctx = EncodeContext<uint32_t>;

  ctx::List fields;
  fields.push_back(ctx::Literal::make(0x91, 0, 7, 24));
  fields.push_back(ctx::Variable::make("a", 12, 15,  0));
  fields.push_back(ctx::Variable::make("a",  8, 11,  8));
  fields.push_back(ctx::Variable::make("a",  4,  7, 16));
  fields.push_back(ctx::Variable::make("a",  0,  3, 20));
  fields.push_back(ctx::Variable::make("b",  6,  9,  4));

  ctx::Store store;
  const auto count = ctx::initialize(store, fields);
  cs::println("Number of Variables = %", count);

  inputVariables(&store);

  const auto value = ctx::compose(fields, store);

  cs::println("0x%", cs::hexf(value, true));
}

////// Parser ////////////////////////////////////////////////////////////////

enum EncodeTokens : cs::tokenid_t {
  TOK_Identifier = cs::Token::make_userid(1),
  TOK_Integral,
  TOK_String
};

class EncoderTokenNames : public cs::BaseTokenNames {
private:
  struct ctor_tag {
    ctor_tag() noexcept = default;
  };

public:
  EncoderTokenNames(const ctor_tag& = ctor_tag()) noexcept
  {
  }

  ~EncoderTokenNames() noexcept = default;

  const char *name(const cs::tokenid_t id) const
  {
    if(        id == TOK_Identifier ) {
      return "Identifier";
    } else if( id == TOK_Integral ) {
      return "Integral";
    } else if( id == TOK_String ) {
      return "String";
    }
    return cs::BaseTokenNames::name(id);
  }

  static cs::TokenNamesPtr make()
  {
    return std::make_unique<EncoderTokenNames>();
  }
};

class EncodeParser : public cs::BaseParser<char> {
public:
  using parser_type = cs::BaseParser<char>;
  using parser_type::char_type;

  using value_type = uint64_t; // TODO

  EncodeParser() noexcept = default;

  ~EncodeParser() noexcept = default;

protected:
  bool initialize()
  {
    using ctx = cs::LexerContext<char_type>;

    _lexer.addScanner(ctx::CharLiteralScanner::make("(),={}[]:@"));
    _lexer.addScanner(ctx::CIdentifierScanner::make(TOK_Identifier));
    _lexer.addScanner(ctx::CIntegralScanner<value_type>::make(TOK_Integral, true));
    _lexer.addScanner(ctx::CStringScanner::make(TOK_String));

    _names = EncoderTokenNames::make();

    return true;
  }

  void start()
  {
    parseEncode();
    check(cs::TOK_EndOfInput);
  }

  /*
   * Grammar:
   *
   * Encode = identifier<'Encode'> '(' integral ',' string ')' '=' '{' Field { ',' Field } '}' .
   */
  void parseEncode()
  {
    if( isLookAheadValue<std::string>(TOK_Identifier, "Encode") ) {
      scan();
    } else {
      throwUnexpectedTokenValue(_lookAheadToken);
    }

    check('(');

    check(TOK_Integral);
    // TODO

    check(',');

    check(TOK_String);
    // TODO

    check(')');

    check('=');

    check('{');

    parseField();

    while( isLookAhead(',') ) {
      scan();

      parseField();
    }

    check('}');
  }

  /*
   * Grammar:
   *
   * Field = ( identifier | integral ) '[' integral ':' integral ']' '@' integral .
   */
  void parseField()
  {
    using Ctx = EncodeContext<value_type>; // TODO

    cs::TokenPtr fieldType;
    if( isLookAhead(TOK_Identifier)  ||  isLookAhead(TOK_Integral) ) {
      scan();
      fieldType = std::move(_currentToken);
    } else {
      throwUnexpectedToken(_lookAheadToken);
    }

    check('[');

    check(TOK_Integral);
    const std::size_t to = currentValue<value_type>();

    check(':');

    check(TOK_Integral);
    const std::size_t from = currentValue<value_type>();

    check(']');

    check('@');

    check(TOK_Integral);
    const std::size_t at = currentValue<value_type>();

    Ctx::Field field = fieldType->id() == TOK_Identifier
        ? Ctx::Variable::make(cs::Token::to_value<std::string>(fieldType),
                              from, to, at)
        : Ctx::Literal::make(cs::Token::to_value<value_type>(fieldType),
                             from, to, at);
    if( !field ) {
      throwErrorMessage(_currentToken->line(), "invalid field definition");
    }
  } // parseField()
};

void test_parser()
{
  EncodeParser parser;

#if 1
  parser.parse("Encode(32,\"Test\") = { field[7:0]@2 , 0xF[1:0]@0 }");
#else
  parser.parse("field[7:0]@2");
  printf("---\n");
  parser.parse("0xF[1:0]@0");
#endif
}

////// Main //////////////////////////////////////////////////////////////////

int main(int /*argc*/, char ** /*argv*/)
{
  test_parser();
  return EXIT_SUCCESS;
}
