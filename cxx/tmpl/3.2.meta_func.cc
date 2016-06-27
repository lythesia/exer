#include <type_traits>
#include <iostream>
using namespace std;

// template template parameter
// template F accept one arg(type) X, and computes output ot F<X>::type
// this do not need wrap for F::template apply<X>::type
template <template <class> class F, class X>
struct apply : F<X> {};
// the same: struct apply { typedef typename F<X>::type type; }; analogy to T apply(F, X) { return F(X); }, but it restrict to F must have ::type

template <template <class> class F, class X>
struct twice : apply<F, typename apply<F,X>::type> {}; // you need to know ::type ..

template <template <class> class F, class X>
struct twice1 : F<typename F<X>::type> {}; // more directly..

int main(int argc, const char *argv[])
{
  static_assert(is_same<int*, apply<add_pointer, int>::type>::value, "int*");
  static_assert(is_same<int**, twice<add_pointer, int>::type>::value, "int**");
  static_assert(is_same<int**, twice1<add_pointer, int>::type>::value, "int**");
  return 0;
}


// boost::mpl::lambda<F>::type ---> F: lambda X F<X> turns into meta func class
