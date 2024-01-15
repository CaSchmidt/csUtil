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

namespace encode {

  using size_t = std::size_t;

  static_assert( std::is_unsigned_v<size_t> );

  namespace impl_encode {

    template<typename T>
    T makeValue(const T x,
                const size_t from, const size_t to,
                const size_t at)
    {
      const T mask = cs::makeBitMask<T>(to - from + 1);

      return ((x >> from) & mask) << at;
    }

  } // namespace impl_encode

  template<typename T>
  class IEncodeValue {
  public:
    using value_type = T;

    IEncodeValue(const size_t from, const size_t to,
                 const size_t at = 0) noexcept
      : _at{at}
      , _from{from}
      , _to{to}
    {
    }

    virtual ~IEncodeValue() noexcept
    {
    }

    virtual bool isValid() const
    {
      constexpr size_t MAX_BIT = sizeof(value_type)*8 - 1;

      return _from <= _to  &&  _to - _from + _at <= MAX_BIT;
    }

    virtual std::string name() const
    {
      return std::string();
    }

    size_t at() const
    {
      return _at;
    }

    size_t from() const
    {
      return _from;
    }

    size_t to() const
    {
      return _to;
    }

    virtual value_type value(const value_type x) const = 0;

  private:
    IEncodeValue() noexcept = delete;

    std::size_t _at{0};
    std::size_t _from{0};
    std::size_t _to{0};
  };

  template<typename ValueT>
  using EncodeValuePtr = std::unique_ptr<IEncodeValue<ValueT>>;

  template<typename ValueT>
  class LiteralValue : public IEncodeValue<ValueT> {
  public:
    using value_type = typename IEncodeValue<ValueT>::value_type;

    LiteralValue(const value_type value,
                 const size_t from, const size_t to,
                 const size_t at) noexcept
      : IEncodeValue<value_type>(from, to, at)
      , _value{value}
    {
    }

    ~LiteralValue() noexcept
    {
    }

    value_type value(const value_type) const
    {
      return impl_encode::makeValue(_value,
                                    IEncodeValue<ValueT>::from(),
                                    IEncodeValue<ValueT>::to(),
                                    IEncodeValue<ValueT>::at());
    }

  private:
    LiteralValue() noexcept = delete;

    value_type _value{};
  };

  template<typename T>
  bool isLiteral(const EncodeValuePtr<T>& ptr)
  {
    return dynamic_cast<LiteralValue<T>*>(ptr.get()) != nullptr;
  }

  template<typename T>
  EncodeValuePtr<T> make_literal(const T value,
                                 const size_t from, const size_t to,
                                 const size_t at = 0)
  {
    EncodeValuePtr<T> ptr = std::make_unique<LiteralValue<T>>(value, from, to, at);
    if( !ptr->isValid() ) {
      printf("no\n");
      ptr.reset();
    }
    return ptr;
  }

  template<typename T>
  class VariableValue : public IEncodeValue<T> {
  public:
    using value_type = typename IEncodeValue<T>::value_type;

    VariableValue(const std::string& name,
                  const size_t from, const size_t to,
                  const size_t at) noexcept
      : IEncodeValue<value_type>(from, to, at)
      , _name(name)
    {
    }

    ~VariableValue() noexcept
    {
    }

    bool isValid() const
    {
      return cs::isIdent(_name)  &&  IEncodeValue<value_type>::isValid();
    }

    std::string name() const
    {
      return _name;
    }

    value_type value(const value_type x) const
    {
      return impl_encode::makeValue(x,
                                    IEncodeValue<T>::from(),
                                    IEncodeValue<T>::to(),
                                    IEncodeValue<T>::at());
    }

  private:
    VariableValue() noexcept = delete;

    std::string _name;
  };

  template<typename T>
  bool isVariable(const EncodeValuePtr<T>& ptr)
  {
    return dynamic_cast<VariableValue<T>*>(ptr.get()) != nullptr;
  }

  template<typename T>
  EncodeValuePtr<T> make_variable(const std::string& name,
                                  const size_t from, const size_t to,
                                  const size_t at = 0)
  {
    EncodeValuePtr<T> ptr = std::make_unique<VariableValue<T>>(name, from, to, at);
    if( !ptr->isValid() ) {
      printf("no\n");
      ptr.reset();
    }
    return ptr;
  }

  template<typename T>
  using EncodeVariables = std::unordered_map<std::string,T>;

  namespace impl_encode {

    template<typename T>
    T getValue(const EncodeVariables<T>& store,
               const EncodeValuePtr<T>& ptr,
               const T defValue = 0)
    {
      if( !ptr ) {
        return defValue;
      }
      const auto hit = store.find(ptr->name());
      return hit != store.end()
          ? hit->second
          : defValue;
    }

  } // namespace impl_encode

  template<typename T>
  using EncodeValues = std::list<EncodeValuePtr<T>>;

} // namespace encode

namespace enc = encode;

void encode32()
{
  using value_type = uint32_t;

  using List = enc::EncodeValues<value_type>;
  using Pointer = enc::EncodeValuePtr<value_type>;
  using Store = enc::EncodeVariables<value_type>;
  using StoreValue = Store::value_type;

  Store store;
  store.insert(StoreValue("a", 0x1234));
  store.insert(StoreValue("b", 0x03C0));

  List list;
  list.push_back(enc::make_literal<value_type>(0x91, 0, 7, 24));
  list.push_back(enc::make_variable<value_type>("a", 12, 15,  0));
  list.push_back(enc::make_variable<value_type>("a",  8, 11,  8));
  list.push_back(enc::make_variable<value_type>("a",  4,  7, 16));
  list.push_back(enc::make_variable<value_type>("a",  0,  3, 20));
  list.push_back(enc::make_variable<value_type>("b",  6,  9,  4));

  value_type value = 0;
  for(const Pointer& ptr : list) {
    value |= ptr->value(enc::impl_encode::getValue(store, ptr));
  }

  cs::println("0x%", cs::hexf(value, true));
}

int main(int /*argc*/, char ** /*argv*/)
{
  encode32();
  return EXIT_SUCCESS;
}
