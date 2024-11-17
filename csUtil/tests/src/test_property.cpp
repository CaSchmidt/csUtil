#include <cstdlib>

#include <print>
#include <type_traits>

#include <cs/Core/MixIn.h>
#include <cs/Text/MixIn.h>

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
    std::println("ctor({})", value);
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
    std::println("ctor({})", impl_cref());
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
  using value_type = int;

  value_type i;

#if 0
  inline const value_type& cref() const
  {
    return i;
  }

  inline value_type& ref()
  {
    return i;
  }
#endif
};

#if 0
using ObjectGetSet = GetSetTraits<Object::value_type,Object,&Object::cref,&Object::ref>;
#else
using ObjectGetSet = ReadWriteMemberTraits<Object::value_type,Object,&Object::i>;
#endif


////// Vector ////////////////////////////////////////////////////////////////

template<typename T> requires std::is_arithmetic_v<T>
struct Vec
    : public cs::ToStringMixIn<Vec<T>> {
  using value_type = T;

  Vec(const value_type x = value_type{},
      const value_type y = value_type{},
      const value_type z = value_type{}) noexcept
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

  value_type x{}, y{}, z{};
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

namespace std {

  template<typename CharT> ///////////////////////////////////////////////////
  struct formatter<IntProperty,CharT>
      : public std::formatter<IntProperty::value_type,CharT> {
    using Formatter = std::formatter<IntProperty::value_type,CharT>;

    template<class FormatContext>
    inline typename FormatContext::iterator format(const IntProperty& prop,
                                                   FormatContext& ctx) const
    {
      const IntProperty::value_type value = prop;
      return Formatter::format(value, ctx);
    }
  };

  template<typename CharT> ///////////////////////////////////////////////////
  struct formatter<Vec<int>,CharT>
      : public cs::ToString_formatter<Vec<int>,CharT> {
  };

  template<typename CharT> ///////////////////////////////////////////////////
  struct formatter<VecProperty<int>,CharT>
      : public std::formatter<std::basic_string_view<CharT>,CharT> {
    using Formatter = std::formatter<std::basic_string_view<CharT>,CharT>;

    template<class FormatContext>
    inline typename FormatContext::iterator format(const VecProperty<int>& prop,
                                                   FormatContext& ctx) const
    {
      const VecProperty<int>::value_type value = prop;

      auto&& out = ctx.out();

      format_to(out, "{}", value);

      return out;
    }
  };

  template<typename CharT> ///////////////////////////////////////////////////
  struct formatter<PropertyRef<ObjectGetSet>,CharT>
      : std::formatter<PropertyRef<ObjectGetSet>::value_type,CharT> {
    using Formatter = std::formatter<PropertyRef<ObjectGetSet>::value_type,CharT>;

    template<class FormatContext>
    inline typename FormatContext::iterator format(const PropertyRef<ObjectGetSet>& prop,
                                                   FormatContext& ctx) const
    {
      const PropertyRef<ObjectGetSet>::value_type value = prop;
      return Formatter::format(value, ctx);
    }
  };

} // namespace std

int main(int /*argc*/, char ** /*argv*/)
{
  {
    IntProperty i;

    i = 7;

    std::println("i = {}", i);
  }

  {
    VecProperty<> v({1, 2, 3});

    v->x = 7;
    v->y = 8;
    v->z = 9;

    std::println("v = {}", v);
  }

  {
    Object o{-1};
    PropertyRef<ObjectGetSet> i{&o};

    i = 7;

    std::println("o.i = {}", i);
  }

  return EXIT_SUCCESS;
}
