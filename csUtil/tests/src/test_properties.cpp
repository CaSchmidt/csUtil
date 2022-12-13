#include <cstdio>
#include <cstdlib>

#include <array>

#if 1
template<typename array_type, std::size_t INDEX>
class ArrayIndexRWProperty {
public:
  using value_type = typename array_type::value_type;

  ArrayIndexRWProperty(array_type& ref) noexcept
    : _ref{ref}
  {
  }

  ~ArrayIndexRWProperty() noexcept = default;

  operator value_type() const
  {
    return _ref[INDEX];
  }

  value_type& operator=(const value_type& val)
  {
    _ref[INDEX] = val;
    return _ref[INDEX];
  }

private:
  ArrayIndexRWProperty() noexcept = default;

  array_type& _ref;
};
#else
template<typename T, std::size_t INDEX,
         typename K,
         T (K::*get)(const std::size_t) const,
         T& (K::*set)(const std::size_t, const T&)>
class ArrayIndexRWProperty {
public:
  using value_type = typename K::value_type;

  ArrayIndexRWProperty(K& obj) noexcept
    : _obj{obj}
  {
  }

  ~ArrayIndexRWProperty() noexcept = default;

  operator value_type() const
  {
    return _obj.get(INDEX);
  }

  value_type& operator=(const value_type& val)
  {
    return _obj.set(INDEX, val);
  }

private:
  ArrayIndexRWProperty() noexcept = delete;

  K& _obj;
};
#endif

struct Vec {
  using  data_type = std::array<float,4>;
  using  size_type = typename data_type::size_type;
  using value_type = typename data_type::value_type;

  Vec(const value_type& val = 0) noexcept
#if 0
    : x{*this}
    , y{*this}
    , z{*this}
    , w{*this}
  #endif
  {
    data[0] = data[1] = data[2] = val;
    data[3] = 1;
  }

  void print() const
  {
    printf("data =");
    for(size_type i = 0; i < data.size(); i++) {
      printf(" %.3f", data[i]);
    }
    printf("\n");
    fflush(stdout);
  }

  value_type get(const size_type index) const
  {
    return data[index];
  }

  value_type& set(const size_type index, const value_type& val)
  {
    data[index] = val;
    return data[index];
  }

  data_type data;

#if 1
  template<size_type INDEX>
  using property_type = ArrayIndexRWProperty<data_type,INDEX>;

  property_type<0> x{data};
  property_type<1> y{data};
  property_type<2> z{data};
  property_type<3> w{data};
#else
  template<size_type INDEX>
  using property_type = ArrayIndexRWProperty<value_type,INDEX,Vec,&Vec::get,&Vec::set>;

  property_type<0> x;
  property_type<1> y;
  property_type<2> z;
  property_type<3> w;
#endif
};

int main(int, char **)
{
  Vec v{7};
  v.print();
  v.x = 2;
  v.y = 3;
  v.z = 4;
  v.print();
  printf("x + w = %.3f\n", v.x + v.w);
  printf("sizeof(v) = %d\n", int(sizeof(v)));
  printf("sizeof(data) = %d\n", int(sizeof(v.data)));

  return EXIT_SUCCESS;
}
