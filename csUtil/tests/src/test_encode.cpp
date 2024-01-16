#include <cstdio>
#include <cstdlib>
#include <cstddef>

#include <list>
#include <memory>
#include <string>
#include <unordered_map>

#include <cs/Core/Bit.h>
#include <cs/Text/StringUtil.h>
#include <cs/Text/PrintFormat.h>
#include <cs/Text/PrintUtil.h>

#include "Encode/Encode.h"

void encode32()
{
  using ctx = EncodeContext<uint32_t>;

  ctx::Store store;
  store["a"] = 0x1234;
  store["b"] = 0x03C0;

  ctx::List fields;
  fields.push_back(ctx::Literal::make(0x91, 0, 7, 24));
  fields.push_back(ctx::Variable::make("a", 12, 15,  0));
  fields.push_back(ctx::Variable::make("a",  8, 11,  8));
  fields.push_back(ctx::Variable::make("a",  4,  7, 16));
  fields.push_back(ctx::Variable::make("a",  0,  3, 20));
  fields.push_back(ctx::Variable::make("b",  6,  9,  4));

  const auto value = ctx::compose(fields, store);

  cs::println("0x%", cs::hexf(value, true));
}

int main(int /*argc*/, char ** /*argv*/)
{
  encode32();
  return EXIT_SUCCESS;
}
