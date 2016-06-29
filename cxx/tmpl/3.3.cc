#include <type_traits>
using namespace std;

template <template <class> class F, class X>
struct twice : F<typename F<X>::type> {};

int main(int argc, const char *argv[])
{
  static_assert(is_same<int****,
                        twice<add_pointer,
                              twice<add_pointer, int>::type>::type>::value,
                "int****");
  return 0;
}
