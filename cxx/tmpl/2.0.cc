#include <iostream>
#include <type_traits>

template <typename T>
struct add_const_ref {
  typedef T const &type;
};

template <typename T>
struct add_const_ref<T&> {
  typedef T &type;
};

int main(int argc, const char *argv[])
{
  std::cout << std::boolalpha << std::is_same<const int&, add_const_ref<int>::type>::value << std::endl;
  std::cout << std::boolalpha << std::is_same<int&, add_const_ref<int&>::type>::value << std::endl;
  return 0;
}
