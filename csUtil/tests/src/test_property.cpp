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

template<typename T, typename ObjectT,
         T ObjectT::* Member>
struct ReadWriteMemberTraits {
  using object_type = ObjectT;
  using  value_type = T;

  inline static const value_type& impl_cref(const object_type *object)
  {
    return object->*Member;
  }

  inline static value_type& impl_ref(object_type *object)
  {
    return object->*Member;
  }
};

template<typename GetSetTraitsT>
class PropertyRef
    : public cs::MemberAccessMixIn<PropertyRef<GetSetTraitsT>,typename GetSetTraitsT::value_type>
    , public cs::ReadAccessMixIn<PropertyRef<GetSetTraitsT>,typename GetSetTraitsT::value_type>
    , public cs::WriteAccessMixIn<PropertyRef<GetSetTraitsT>,typename GetSetTraitsT::value_type> {
public:
  using traits_type = GetSetTraitsT;
  using object_type = typename traits_type::object_type;
  using  value_type = typename traits_type::value_type;

  explicit PropertyRef(object_type *object)
    : _object{object}
  {
    std::cout << "ctor(" << impl_cref() << ")" << std::endl;
  }

  /*
   * NOTE: The copy & move ctor cannot be implemented,
   *       as the this->_object is not initialized!
   */

  PropertyRef(const PropertyRef&) = delete;

  inline PropertyRef& operator=(const PropertyRef& other)
  {
    impl_ref() = other.impl_cref();
    return *this;
  }

  PropertyRef(PropertyRef&&) = delete;

  inline PropertyRef& operator=(PropertyRef&& other)
  {
    impl_ref() = std::move(other.impl_ref());
    return *this;
  }

  ~PropertyRef() = default;

  using cs::WriteAccessMixIn<PropertyRef<traits_type>,value_type>::operator=;

private:
  friend cs::MemberAccessMixIn<PropertyRef<traits_type>,value_type>;
  friend cs::ReadAccessMixIn<PropertyRef<traits_type>,value_type>;
  friend cs::WriteAccessMixIn<PropertyRef<traits_type>,value_type>;

  inline const value_type& impl_cref() const
  {
    return traits_type::impl_cref(_object);
  }

  inline value_type& impl_ref()
  {
    return traits_type::impl_ref(_object);
  }

  object_type *_object{nullptr};
};

////// Object ////////////////////////////////////////////////////////////////

struct Object {
  int i;

#if 0
  inline const int& int_cref() const
  {
    return i;
  }

  inline int& int_ref()
  {
    return i;
  }
#endif
};

#if 1
using ObjectGetSet = ReadWriteMemberTraits<int,Object,&Object::i>;
#else
using ObjectGetSet = GetSetTraits<int,Object,&Object::int_cref,&Object::int_ref>;
#endif

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

////// Access ////////////////////////////////////////////////////////////////

int prop_get(const Property<int>& prop)
{
  return prop;
}

void prop_set(Property<int>& prop, const int value)
{
  prop = value;
}

//////

int prop_get(const Property<Vec<int>>& prop)
{
  return prop->y;
}

void prop_set(Property<Vec<int>>& prop, const int value)
{
  prop->y = value;
}

//////

int prop_get(const PropertyRef<ObjectGetSet>& prop)
{
  return prop;
}

void prop_set(PropertyRef<ObjectGetSet>& prop, const int value)
{
  prop = value;
}

////// Main //////////////////////////////////////////////////////////////////

using IntProperty = Property<int>;

template<typename T = int>
using VecProperty = Property<Vec<T>>;

int main(int /*argc*/, char ** /*argv*/)
{
  {
    IntProperty i;

    i = 7;

    std::cout << "i = " << i << std::endl;
  }

  {
    VecProperty<> v({1, 2, 3});

    v->x = 7;
    v->y = 8;
    v->z = 9;

    std::cout << "v = " << v << std::endl;
  }

  {
    Object o{-1};
    PropertyRef<ObjectGetSet> i{&o};

    i = 7;

    std::cout << "o.i = " << i << std::endl;
  }

  return EXIT_SUCCESS;
}
