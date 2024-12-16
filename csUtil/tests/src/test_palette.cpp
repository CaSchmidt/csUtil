#include <cstdlib>

#include <algorithm>
#include <array>
#include <format>
#include <print>
#include <string>

/*
 * "Rich Code for Tiny Computers", Jason Turner, CppCon 2016
 *
 * Compile with GCC: -Wall -O2 -std=c++23
 */

#undef HAVE_PRINT

struct Color {
  using arith_t = int;
  using color_t = unsigned char;

  constexpr Color(const color_t index,
                  const color_t r,
                  const color_t g,
                  const color_t b) noexcept
    : index{index}
    , r{r}
    , g{g}
    , b{b}
  {
  }

  color_t index, r, g, b;

  static constexpr arith_t distance(const Color& from, const Color& to)
  {
    const arith_t dr = arith_t{to.r} - arith_t{from.r};
    const arith_t dg = arith_t{to.g} - arith_t{from.g};
    const arith_t db = arith_t{to.b} - arith_t{from.b};

    return dr*dr + dg*dg + db*db;
  }

  constexpr arith_t distance(const Color& c) const
  {
    return distance(*this, c);
  }

#ifdef HAVE_PRINT
  inline std::string toString() const
  {
    return std::format("#{:02X}{:02X}{:02X}", r, g, b);
  }
#endif
};

template<typename DerivedT>
struct IPalette {
  using color_t = Color::color_t;

  static constexpr auto size()
  {
    return DerivedT::COLORS.size();
  }

  static constexpr const Color& find(const Color& c)
  {
    const auto lambda_cmp = [&](const Color& a, const Color& b) -> bool {
      return a.distance(c) < b.distance(c);
    };

    return *std::min_element(DerivedT::COLORS.begin(),
                             DerivedT::COLORS.end(),
                             lambda_cmp);
  }

  static constexpr const Color& find(const color_t r,
                                     const color_t g,
                                     const color_t b)
  {
    return find(Color{0, r, g, b});
  }
};

struct EGA16Palette : IPalette<EGA16Palette> {
  static constexpr std::array<Color,16> COLORS{
    Color{ 0, 0x00, 0x00, 0x00},
    Color{ 1, 0x00, 0x00, 0xAA},
    Color{ 2, 0x00, 0xAA, 0x00},
    Color{ 3, 0x00, 0xAA, 0xAA},
    Color{ 4, 0xAA, 0x00, 0x00},
    Color{ 5, 0xAA, 0x00, 0xAA},
    Color{ 6, 0xAA, 0x55, 0x00},
    Color{ 7, 0xAA, 0xAA, 0xAA},
    Color{ 8, 0x55, 0x55, 0x55},
    Color{ 9, 0x55, 0x55, 0xFF},
    Color{10, 0x55, 0xFF, 0x55},
    Color{11, 0x55, 0xFF, 0xFF},
    Color{12, 0xFF, 0x55, 0x55},
    Color{13, 0xFF, 0x55, 0xFF},
    Color{14, 0xFF, 0xFF, 0x55},
    Color{15, 0xFF, 0xFF, 0xFF}
  };
};

volatile Color::color_t index = 255;

Color::color_t find(const Color& c)
{
  const Color& nearest = EGA16Palette::find(c);

#ifdef HAVE_PRINT
  std::println("{:2}: {}", nearest.index, nearest.toString());
#endif

  return nearest.index;
}

int main(int /*argc*/, char ** /*argv*/)
{
  index = EGA16Palette::find(0, 0, 0).index;

  std::println("size = {}", EGA16Palette::size());

  for(std::size_t i = 0; i < EGA16Palette::size(); i++) {
    find(EGA16Palette::COLORS[i]);
  }

  return EXIT_SUCCESS;
}
