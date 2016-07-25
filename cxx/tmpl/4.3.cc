#include <boost/mpl/apply.hpp>
#include <boost/mpl/next.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/equal.hpp>
#include <boost/mpl/not_equal_to.hpp>
#include <boost/mpl/greater.hpp>
#include <boost/mpl/minus.hpp>
#include <boost/mpl/plus.hpp>
#include <boost/mpl/multiplies.hpp>

// #include <typeinfo>
// #include <iostream>
// using namespace std;

namespace mpl = boost::mpl;

// probe
template <class X> struct fail_on_inst;

/* 1 */
template <typename N, typename Pred>
struct next_if : mpl::eval_if<mpl::apply<Pred,N>, mpl::next<N>, N>::type {}; // no identity is ok

template <typename N, typename Pred>
struct test_if : mpl::eval_if<mpl::apply<Pred,N>, fail_on_inst<N>, mpl::identity<N>>::type {}; // just test for lazy inst

struct always {
  template <typename N> 
    struct apply : mpl::true_ {};
};

struct never {
  template <typename N> 
    struct apply : mpl::false_ {};
};

/* 2 */
template <typename N1, typename N2>
struct formula :
  mpl::eval_if<
    mpl::not_equal_to<N1, N2>,
    mpl::eval_if<mpl::greater<N1, N2>, mpl::minus<N1, N2>, N1>,
    mpl::plus<N1, mpl::multiplies<N1, mpl::int_<2>>>
  >::type {};

int main(int argc, const char *argv[])
{
  // mpl::apply<is_odd<mpl::_>, mpl::int_<1>>::type t;     // true_
  // mpl::apply<mpl::next<mpl::_>, mpl::int_<1>>::type t2; // int_<2>

  static_assert(next_if<mpl::int_<1>, always>::value == 2, "true");
  static_assert(test_if<mpl::int_<2>, never>::value == 2, "true");

  static_assert(formula<mpl::int_<5>, mpl::int_<4>>::value == 1, "true");
  return 0;
}
