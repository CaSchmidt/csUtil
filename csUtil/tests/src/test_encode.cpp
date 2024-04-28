#include <cstdio>
#include <cstdlib>
#include <cstddef>

#include <charconv>

#include <cs/Lexer/Context.h>
#include <cs/Lexer/Parser.h>
#include <cs/Text/PrintFormat.h>
#include <cs/Text/PrintUtil.h>

#include "Encode/Parser.h"

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
void inputVariables(Encode::VariableStore<T>& store)
{
  using citer_t = typename Encode::VariableStore<T>::const_iterator;

  for(citer_t it = store.cbegin(); it != store.cend(); ++it) {
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

    store[it->first] = value;
  }
}

void test_encode32()
{
  using EnginePtr = Encode::EnginePtr<uint32_t>;
  using Engine    = EnginePtr::element_type;
  using Store     = Engine::Store;

  using  size_type = Engine::size_type;
  using value_type = Engine::value_type;

  EnginePtr engine = Engine::make(32, "test_encode32");
  engine->addLiteral(0x91, 0, 7, 24);
  engine->addVariable("a", 12, 15,  0);
  engine->addVariable("a",  8, 11,  8);
  engine->addVariable("a",  4,  7, 16);
  engine->addVariable("a",  0,  3, 20);
  engine->addVariable("b",  6,  9,  4);

  Store store;
  const size_type count = engine->initialize(store);
  cs::println("Number of Variables = %", count);

  inputVariables(store);

  const value_type value = engine->compose(store);

  cs::println("%: 0x%", engine->text(), cs::hexf(value, true));
}

void test_parser()
{
  Encode::Parser<uint32_t> parser;

  parser.parse("Encode(32,\"Test\") = { field[7:0]@2 , 0xF[1:0]@0 }\n");
}

////// Main //////////////////////////////////////////////////////////////////

int main(int /*argc*/, char ** /*argv*/)
{
  test_encode32();

  return EXIT_SUCCESS;
}
