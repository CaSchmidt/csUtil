#include <iostream>

#include <catch.hpp>

#include <N4/N4.h>
#include <N4/Optics.h>
#include <N4/Util.h>

////// Global Types //////////////////////////////////////////////////////////

using n4::real_t;

using Vec4f = n4::Vertex4f;

constexpr real_t W0 = Vec4f::traits_type::have_w ? 1 : 0;

using Mat4f = n4::Matrix4f;

////// Output ////////////////////////////////////////////////////////////////

namespace test_print {

  inline void do_print(const real_t x)
  {
    if( n4::abs(x) == real_t(0) ) {
      printf("%8.3f", 0.0);
    } else {
      printf("%8.3f", x);
    }
  }

  void print(const real_t v, const char *ident = nullptr)
  {
    if( ident != nullptr ) {
      printf("%s = \n", ident);
    }
    do_print(v); printf("\n");
    printf("\n");
    fflush(stdout);
  }

  void print(const Vec4f& v, const char *ident = nullptr)
  {
    if( ident != nullptr ) {
      printf("%s =\n", ident);
    }
    for(std::size_t i = 0; i < v.size(); i++) {
      do_print(v(i)); printf("\n");
    }
    printf("\n");
    fflush(stdout);
  }

  void print(const Mat4f& M, const char *ident = nullptr)
  {
    if( ident != nullptr ) {
      printf("%s =\n", ident);
    }
    for(std::size_t i = 0; i < M.rows(); i++) {
      for(std::size_t j = 0; j < M.columns(); j++) {
        printf("  "); do_print(M(i, j));
      }
      printf("\n");
    }
    printf("\n");
    fflush(stdout);
  }

  void print(const char *s, const double d)
  {
    printf("%-16s = %26.24f (%a)\n", s, d, d);
  }

} // namespace test_print

#define PRINTreal(real)  test_print::print(real, #real)
#define PRINTexpr(expr)  test_print::print(expr, #expr)
#define PRINTmatr(matr)  test_print::print(matr, #matr)

////// Test Utilities ////////////////////////////////////////////////////////

namespace test_konst {

  constexpr real_t epsilon0 = 0x1p-20;

} // namespace test_konst

namespace test_equal {

  bool equals(const real_t a, const real_t b,
              const real_t epsilon0 = test_konst::epsilon0)
  {
    return n4::isZero(a - b, epsilon0);
  }

  bool equals(const Vec4f& v, const std::initializer_list<real_t>& list,
              const real_t epsilon0 = test_konst::epsilon0)
  {
    if( v.size() != list.size() ) {
      return false;
    }
    for(std::size_t i = 0; i < v.size(); i++) {
      if( !equals(v(i), list.begin()[i], epsilon0) ) {
        return false;
      }
    }
    return true;
  }

  bool equals(const Mat4f& M, const std::initializer_list<real_t>& list,
              const real_t epsilon0 = test_konst::epsilon0)
  {
    if( M.size() != list.size() ) {
      return false;
    }
    for(std::size_t l = 0; l < list.size(); l++) {
      // NOTE: Row-Major Notation: l = i*4 + j
      const std::size_t i = l/4;
      const std::size_t j = l%4;
      if( !equals(M(i, j), list.begin()[l], epsilon0) ) {
        return false;
      }
    }
    return true;
  }

  bool equals(const Mat4f& A, const Mat4f& B,
              const real_t epsilon0 = test_konst::epsilon0)
  {
    for(std::size_t l = 0; l < A.size(); l++) {
      if( !equals(A.data()[l], B.data()[l], epsilon0) ) {
        return false;
      }
    }
    return true;
  }

} // namespace test_equal

////// Test Cases ////////////////////////////////////////////////////////////

namespace test_n4 {
  using namespace test_equal;

  // Begin Global Constants //////////////////////////////////////////////////

  const Vec4f a{1, 2, 3};
  const Vec4f b{2, 3, 4};

  const Vec4f delta3{1, 2, 2}; // length == 3
  const Vec4f delta5{0, 3, 4}; // length == 5
  const Vec4f delta7{2, 3, 6}; // length == 7

  const Mat4f M{
     0,  1,  2,  3,
     4,  5,  6,  7,
     8,  9, 10, 11,
    12, 13, 14, 15
  };

  // End Global Constants ////////////////////////////////////////////////////

  TEST_CASE("N4 global constants.", "[N4][global]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    PRINTexpr(a);
    PRINTexpr(b);
    PRINTreal(W0);
    PRINTmatr(M);

    test_print::print("0.0001", 0x1p-13);
    test_print::print("0.000001", 0x1p-20);
    test_print::print("epsilon0", test_konst::epsilon0);
  }

  TEST_CASE("N4 global base.", "[N4][base]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    {
      Vec4f v_set(5);
      REQUIRE( equals(v_set, {5, 5, 5, W0}, 0) );

      v_set = 7;
      REQUIRE( equals(v_set, {7, 7, 7, W0}, 0) );
    }

    {
      Vec4f v_list{2, 4, 6};
      REQUIRE( equals(v_list, {2, 4, 6, W0}, 0) );

      v_list = {3, 5, 7};
      REQUIRE( equals(v_list, {3, 5, 7, W0}, 0) );
    }

    {
      Vec4f v_copy(a);
      REQUIRE( equals(v_copy, {1, 2, 3, W0}, 0) );

      v_copy = b;
      REQUIRE( equals(v_copy, {2, 3, 4, W0}, 0) );
    }

    {
      Vec4f a{3, 5, 7};
      Vec4f v_move(std::move(a));
      REQUIRE( equals(v_move, {3, 5, 7, W0}, 0) );

      Vec4f b{5, 7, 9};
      v_move = std::move(b);
      REQUIRE( equals(v_move, {5, 7, 9, W0}, 0) );
    }
  }

  TEST_CASE("N4 Vector4f assignment.", "[Vector4f][assign]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    REQUIRE( equals(Vec4f(a) += b, {3, 5, 7, W0}          , 0) );
    REQUIRE( equals(Vec4f(b) -= a, {1, 1, 1, W0}          , 0) );
    REQUIRE( equals(Vec4f(b) *= a, {2, 6, 12, W0}         , 0) );
    REQUIRE( equals(Vec4f(a) *= 2, {2, 4, 6, W0}          , 0) );
    REQUIRE( equals(Vec4f(b) /= a, {2, 1.5, 1.333333f, W0}   ) );
    REQUIRE( equals(Vec4f(b) /= 8, {0.25, 0.375, 0.5, W0} , 0) );

    {
      Vec4f v1;
      v1 += a;
      v1 += b;
      v1 -= Vec4f{1, 1, 1};
      v1 /= 2;
      REQUIRE( equals(v1, {1, 2, 3, W0}, 0) );
    }
  }

  TEST_CASE("N4 Vector4f binary operators.", "[Vector4f][binary]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    REQUIRE( equals(a + b    , {3, 5, 7, W0}                              , 0) );
    REQUIRE( equals(a - b    , {-1, -1, -1, W0}                           , 0) );
    REQUIRE( equals(a * b    , {2, 6, 12, W0}                             , 0) );
    REQUIRE( equals(a / b    , {0.5, 0.666666f, 0.75, W0}                    ) );
    REQUIRE( equals(2*a - a  , {1, 2, 3, W0}                              , 0) );
    REQUIRE( equals(a*2 - a  , {1, 2, 3, W0}                              , 0) );
    REQUIRE( equals(4*a/2 - a, {1, 2, 3, W0}                              , 0) );
    REQUIRE( equals(6/a,       {6, 3, 2, W0}                              , 0) );
    REQUIRE( equals(n4::translate(1, 1, 1)*a, {1 + W0, 2 + W0, 3 + W0, W0}, 0) );
  }

  TEST_CASE("N4 Vector4f functions.", "[Vector4f][functions]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    const Vec4f A{1, 3, 3};
    const Vec4f B{2, 2, 4};

    REQUIRE( equals(n4::abs(Vec4f{-1, 2, -3})   , {1, 2, 3, W0}         , 0) );
    REQUIRE( equals(n4::cross(a, b)             , {-1, 2, -1, W0}       , 0) );
    REQUIRE( equals(n4::dot(a, b)               , 20                    , 0) );
    REQUIRE( equals(n4::length(delta3)          , 3)                         );
    REQUIRE( equals(n4::distance(a, a + delta7) , 7)                         );
    REQUIRE( equals(n4::direction(a + delta5, a), {0, -0.6f, -0.8f, W0})     );
    REQUIRE( equals(n4::normalize(delta5)       , {0, 0.6f, 0.8f, W0})       );
    REQUIRE( equals(n4::length(n4::normalize(a)), 1)                         );
    REQUIRE( equals(n4::clamp(a, 1.5, 2.5)      , {1.5, 2, 2.5, W0}     , 0) );
    REQUIRE( equals(n4::max(a, 1.5)             , {1.5, 2, 3, W0}       , 0) );
    REQUIRE( equals(n4::max(1.5, a)             , {1.5, 2, 3, W0}       , 0) );
    REQUIRE( equals(n4::max(A, B)               , {2, 3, 4, W0}         , 0) );
    REQUIRE( equals(n4::min(a, 2.5)             , {1, 2, 2.5, W0}       , 0) );
    REQUIRE( equals(n4::min(2.5, a)             , {1, 2, 2.5, W0}       , 0) );
    REQUIRE( equals(n4::min(A, B)               , {1, 2, 3, W0}         , 0) );
    REQUIRE( equals(a.max()                     , 3                     , 0) );
    REQUIRE( equals(a.min()                     , 1                     , 0) );
    REQUIRE( !a.isZero() );
    REQUIRE( !a.isZero(2) );
    REQUIRE( a.isZero(3) );
    REQUIRE( a.isZero(4) );
    REQUIRE( simd::compareLEQ(a.eval(), b.eval()) );
    REQUIRE( simd::compareLEQ(a.eval(), a.eval()) );
    REQUIRE( !simd::compareLEQ(b.eval(), a.eval()) );
  }

  TEST_CASE("N4 Vector4f manipulators.", "[Vector4f][manipulator]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    n4::Vertex4f v;
    v.x = 1;
    v.y = 2;
    v.z = 3;
    v.w = 4;

    REQUIRE( equals(n4::expr_cast_assign_w<Vec4f::traits_type>(v), {1, 2, 3, 4}, 0) );

    n4::Normal3f n;
    n.x = 1;
    n.y = 2;
    n.z = 3;

    REQUIRE( equals(n4::expr_cast_assign_w<Vec4f::traits_type>(n), {1, 2, 3, 0}, 0) );
  }

  TEST_CASE("N4 Vector4f unary operators.", "[Vector4f][unary]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    REQUIRE( equals(+a, {1, 2, 3, W0}   , 0) );
    REQUIRE( equals(-a, {-1, -2, -3, W0}, 0) );
  }

  TEST_CASE("N4 Matrix4f binary operators.", "[Matrix4f][binary]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    REQUIRE( equals(M*M, { 56,  62,  68,  74,
                          152, 174, 196, 218,
                          248, 286, 324, 362,
                          344, 398, 452, 506 }, 0) );
  }

  TEST_CASE("N4 Matrix4f functions.", "[Matrix4f][functions]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    REQUIRE( equals(M.transpose(), { 0, 4,  8, 12,
                                     1, 5,  9, 13,
                                     2, 6, 10, 14,
                                     3, 7, 11, 15 }) );

    const Mat4f M1{
      1, 0, 0, 0,
      0, 0, 1, 0,
      0, 1, 0, 0,
      0, 0, 0, 1
    };
    PRINTmatr(M1);

    REQUIRE( equals(M1.inverse(), { 1, 0, 0, 0,
                                    0, 0, 1, 0,
                                    0, 1, 0, 0,
                                    0, 0, 0, 1 }) );

    const Mat4f M2{
      4, 0, 0, 0,
      0, 0, 2, 0,
      0, 1, 2, 0,
      1, 0, 0, 1
    };
    PRINTmatr(M2);

    REQUIRE( equals(M2.inverse(), { 0.25, 0  , 0, 0,
                                    0   ,-1  , 1, 0,
                                    0   , 0.5, 0, 0,
                                   -0.25, 0  , 0, 1 }) );

    const Mat4f Rx = n4::rotateXbyPI2(1);
    PRINTmatr(Rx);

    REQUIRE( equals(Rx.inverse(), Rx.transpose()) );

    const Mat4f TS = n4::translate(3, 5, 7)*n4::scale(2, 4, 8);
    PRINTmatr(TS);

    REQUIRE( equals(TS.inverse(), { 0.5, 0   , 0    , -1.5  ,
                                    0  , 0.25, 0    , -1.25 ,
                                    0  , 0   , 0.125, -0.875,
                                    0  , 0   , 0    ,  1     }, 0) );

    REQUIRE( !M.isZero() );
    REQUIRE( !M.isZero(14) );
    REQUIRE( M.isZero(15) );
    REQUIRE( M.isZero(16) );
  }

} // namespace test_n4

namespace test_intersect {

  bool test_intersect(const Vec4f& min, const Vec4f& max,
                      const Vec4f& org, const Vec4f& dir, const real_t tMax)
  {
    return simd::intersectRayAABBox(min.eval(), max.eval(), org.eval(), dir.eval(), tMax);
  }

  bool test_intersect(const Vec4f& min, const Vec4f& max,
                      const Vec4f& org, const Vec4f& dir)
  {
    return simd::intersectRayAABBox(min.eval(), max.eval(), org.eval(), dir.eval());
  }

  TEST_CASE("Ray/Box intersection.", "[intersect]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    const Vec4f min{1, 1, 1};
    const Vec4f max{2, 2, 2};

    // Finite tMax ///////////////////////////////////////////////////////////

    REQUIRE(  test_intersect(min, max, {1.5, 1.5, 0}, {0, 0,  1}, 1.5 ) );
    REQUIRE(  test_intersect(min, max, {1.5, 1.5, 3}, {0, 0, -1}, 1.5 ) );
    REQUIRE( !test_intersect(min, max, {1.5, 1.5, 0}, {0, 0,  1}, 0.75) );
    REQUIRE( !test_intersect(min, max, {1.5, 1.5, 3}, {0, 0, -1}, 0.75) );

    REQUIRE(  test_intersect(min, max, {1.5, 0, 1.5}, {0,  1, 0}, 1.5 ) );
    REQUIRE(  test_intersect(min, max, {1.5, 3, 1.5}, {0, -1, 0}, 1.5 ) );
    REQUIRE( !test_intersect(min, max, {1.5, 0, 1.5}, {0,  1, 0}, 0.75) );
    REQUIRE( !test_intersect(min, max, {1.5, 3, 1.5}, {0, -1, 0}, 0.75) );

    REQUIRE(  test_intersect(min, max, {0, 1.5, 1.5}, { 1, 0, 0}, 1.5  ) );
    REQUIRE(  test_intersect(min, max, {3, 1.5, 1.5}, {-1, 0, 0}, 1.5  ) );
    REQUIRE( !test_intersect(min, max, {0, 1.5, 1.5}, { 1, 0, 0}, 0.75 ) );
    REQUIRE( !test_intersect(min, max, {3, 1.5, 1.5}, {-1, 0, 0}, 0.75 ) );

    REQUIRE( !test_intersect(min, max, {0, 0, 0}, {1, 1, 1}, 1) );
    REQUIRE(  test_intersect(min, max, {0, 0, 0}, {1, 1, 1}, 2) );
    REQUIRE(  test_intersect(min, max, {1, 1, 1}, {1, 1, 1}, 1) );
    REQUIRE(  test_intersect(min, max, {1, 1, 1}, {1, 1, 1}, 2) );
    REQUIRE(  test_intersect(min, max, {1.5, 1.5, 1.5}, {1, 1, 1}, 1) );
    REQUIRE(  test_intersect(min, max, {1.5, 1.5, 1.5}, {1, 1, 1}, 2) );
    REQUIRE(  test_intersect(min, max, {2, 2, 2}, {1, 1, 1}, 1) );
    REQUIRE(  test_intersect(min, max, {2, 2, 2}, {1, 1, 1}, 2) );
    REQUIRE( !test_intersect(min, max, {3, 3, 3}, {1, 1, 1}, 1) );
    REQUIRE( !test_intersect(min, max, {3, 3, 3}, {1, 1, 1}, 2) );

    REQUIRE( !test_intersect(min, max, {3, 3, 3}, {-1, -1, -1}, 1) );
    REQUIRE(  test_intersect(min, max, {3, 3, 3}, {-1, -1, -1}, 2) );
    REQUIRE(  test_intersect(min, max, {2, 2, 2}, {-1, -1, -1}, 1) );
    REQUIRE(  test_intersect(min, max, {2, 2, 2}, {-1, -1, -1}, 2) );
    REQUIRE(  test_intersect(min, max, {1.5, 1.5, 1.5}, {-1, -1, -1}, 1) );
    REQUIRE(  test_intersect(min, max, {1.5, 1.5, 1.5}, {-1, -1, -1}, 2) );
    REQUIRE(  test_intersect(min, max, {1, 1, 1}, {-1, -1, -1}, 1) );
    REQUIRE(  test_intersect(min, max, {1, 1, 1}, {-1, -1, -1}, 2) );
    REQUIRE( !test_intersect(min, max, {0, 0, 0}, {-1, -1, -1}, 1) );
    REQUIRE( !test_intersect(min, max, {0, 0, 0}, {-1, -1, -1}, 2) );

    // Infinite //////////////////////////////////////////////////////////////

    REQUIRE( test_intersect(min, max, {1.5, 1.5, 0}, {0, 0,  1}) );
    REQUIRE( test_intersect(min, max, {1.5, 1.5, 3}, {0, 0, -1}) );

    REQUIRE( test_intersect(min, max, {1.5, 0, 1.5}, {0,  1, 0}) );
    REQUIRE( test_intersect(min, max, {1.5, 3, 1.5}, {0, -1, 0}) );

    REQUIRE( test_intersect(min, max, {0, 1.5, 1.5}, { 1, 0, 0}) );
    REQUIRE( test_intersect(min, max, {3, 1.5, 1.5}, {-1, 0, 0}) );

    REQUIRE(  test_intersect(min, max, {0, 0, 0}, {1, 1, 1}) );
    REQUIRE(  test_intersect(min, max, {1, 1, 1}, {1, 1, 1}) );
    REQUIRE(  test_intersect(min, max, {1.5, 1.5, 1.5}, {1, 1, 1}) );
    REQUIRE(  test_intersect(min, max, {2, 2, 2}, {1, 1, 1}) );
    REQUIRE( !test_intersect(min, max, {3, 3, 3}, {1, 1, 1}) );

    REQUIRE(  test_intersect(min, max, {3, 3, 3}, {-1, -1, -1}) );
    REQUIRE(  test_intersect(min, max, {2, 2, 2}, {-1, -1, -1}) );
    REQUIRE(  test_intersect(min, max, {1.5, 1.5, 1.5}, {-1, -1, -1}) );
    REQUIRE(  test_intersect(min, max, {1, 1, 1}, {-1, -1, -1}) );
    REQUIRE( !test_intersect(min, max, {0, 0, 0}, {-1, -1, -1}) );
  }

} // namespace test_intersect

namespace test_optics {

  using namespace test_equal;

  struct DirectionTraits {
    static constexpr bool have_w = false;
  };

  using Direction = n4::Vector4f<DirectionTraits,n4::Normal3fManipulator>;

  struct NormalTraits {
    static constexpr bool have_w = false;
  };

  using Normal = n4::Vector4f<NormalTraits,n4::Normal3fManipulator>;

  Direction make_I()
  {
    const real_t y = -0.5;
    const real_t x = n4::sqrt(std::max<real_t>(0, n4::optics::ONE - y*y));
    return Direction{x, y, 0};
  }

  TEST_CASE("Optical reflection.", "[optics][reflect]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    const Direction I = make_I();
    REQUIRE( equals(n4::length(I), 1) );

    const Normal N{0, 1, 0};
    REQUIRE( equals(n4::length(N), 1) );

    const Direction R = n4::optics::reflect(I, N);
    REQUIRE( equals(n4::expr_cast<Vec4f::traits_type>(R), {I(0), -I(1), I(2), W0}, 0) );
  }

  TEST_CASE("Optical refraction.", "[optics][refract]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    const Direction I = make_I();
    REQUIRE( equals(n4::length(I), 1) );

    const Normal N{0, 1, 0};
    REQUIRE( equals(n4::length(N), 1) );

    {
      const Direction T1 = n4::optics::refract(I, N, 1);
      REQUIRE( equals(n4::expr_cast<Vec4f::traits_type>(T1), {I(0), I(1), I(2), W0}, 0) );
    }

    {
      const Direction T2 = n4::optics::refract(I, N, 1.5);
      REQUIRE( equals(n4::expr_cast<Vec4f::traits_type>(T2), {0, 0, 0, W0}, 0) );
    }
  }

} // namespace test_optics

namespace test_util {

  using namespace test_equal;

  TEST_CASE("Frame from axis.", "[util][frame]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    REQUIRE( equals(n4::util::frameFromZ(Vec4f{0, 0, 1}), { 0, 1, 0, 0,
                                                           -1, 0, 0, 0,
                                                            0, 0, 1, 0,
                                                            0, 0, 0, 1 }, 0) );
  }

} // namespace test_util
