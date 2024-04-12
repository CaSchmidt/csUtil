#include <cstdio>
#include <cstdlib>

#include <iostream>
#include <type_traits>

#include <cs/Core/MixIn.h>
#include <cs/Text/MixIn.h>
#include <cs/Text/PrintUtil.h>

////// Property //////////////////////////////////////////////////////////////

template<typename T>
class Property
    : public cs::MemberAccessMixIn<Property<T>,T>
    , public cs::ReadAccessMixIn<Property<T>,T>
    , public cs::WriteAccessMixIn<Property<T>,T> {
public:
  using value_type = T;

  explicit Property(const value_type& value = value_type())
    : _value(value)
  {
    std::cout << "ctor(" << value << ")" << std::endl;
  }

  Property(const Property&) = default;
  Property& operator=(const Property&) = default;

  Property(Property&&) = default;
  Property& operator=(Property&&) = default;

  ~Property() = default;

  using cs::WriteAccessMixIn<Property<T>,T>::operator=;

private:
  friend cs::MemberAccessMixIn<Property<T>,T>;
  friend cs::ReadAccessMixIn<Property<T>,T>;
  friend cs::WriteAccessMixIn<Property<T>,T>;

  inline const value_type& impl_cref() const
  {
    return _value;
  }

  inline value_type& impl_ref()
  {
    return _value;
  }

  value_type _value{};
};

template<typename T>
std::ostream& operator<<(std::ostream& stream,
                         const Property<T>& prop)
{
  using value_type = typename Property<T>::value_type;

  stream << static_cast<value_type>(prop);
  return stream;
}

////// PropertyRef ///////////////////////////////////////////////////////////

template<typename T, typename ObjectT,
         const T& (ObjectT::*CRef)() const,
         T& (ObjectT::*Ref)()>
struct GetSetTraits {
  using object_type = ObjectT;
  using  value_type = T;

  inline static const value_type& impl_cref(const object_type *object)
  {
    return (object->*CRef)();
  }

  inline static value_type& impl_ref(object_type *object)
  {
    return (object->*Ref)();
  }
};

////// Vector ////////////////////////////////////////////////////////////////

template<typename T> requires std::is_arithmetic_v<T>
struct Vec
    : public cs::ToStringMixIn<Vec<T>> {
  Vec(const T x = T{}, const T y = T{}, const T z = T{}) noexcept
    : x{x}
    , y{y}
    , z{z}
  {
  }

  Vec(const Vec&) noexcept = default;
  Vec& operator=(const Vec&) noexcept = default;

  Vec(Vec&&) noexcept = default;
  Vec& operator=(Vec&&) noexcept = default;

  auto elements() const
  {
    return std::forward_as_tuple(x, y, z);
  }

  T x{}, y{}, z{};
};

////// Main //////////////////////////////////////////////////////////////////

using IntProperty = Property<int>;

template<typename T = int>
using VecProperty = Property<Vec<T>>;

int main(int /*argc*/, char ** /*argv*/)
{
  IntProperty i;
  i = 7;

  std::cout << "i = " << i << std::endl;

  VecProperty<> v({1, 2, 3});
  v->x = 7;
  v->y = 8;
  v->z = 9;

  std::cout << "v = " << v << std::endl;

  return EXIT_SUCCESS;
}
