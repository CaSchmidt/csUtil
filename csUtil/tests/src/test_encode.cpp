#include <cstdio>
#include <cstdlib>
#include <cstddef>

#include <charconv>

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
void encode32()
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

int main(int /*argc*/, char ** /*argv*/)
{
  encode32();
  return EXIT_SUCCESS;
}
