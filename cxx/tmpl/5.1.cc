#include <boost/mpl/vector_c.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/erase.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/plus.hpp>
#include <boost/mpl/minus.hpp>
#include <boost/mpl/divides.hpp>
#include <boost/mpl/insert_range.hpp>
#include <boost/mpl/equal.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/greater.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/mpl/deref.hpp>
#include <boost/mpl/next.hpp>
#include <boost/mpl/push_back.hpp>
#include <boost/mpl/clear.hpp>
#include <boost/mpl/identity.hpp>

using namespace boost;

// 1. mpl aux
// template <class T>
// struct double_first_half {
//   typedef typename mpl::begin<T>::type iter_begin;
//   typedef typename mpl::end<T>::type iter_end;
//   typedef typename mpl::divides<typename mpl::size<T>::type, mpl::int_<2>>::type half;
//   typedef typename mpl::advance<iter_begin, half>::type iter_mid;
//   typedef typename mpl::transform<typename mpl::erase<T, iter_mid, iter_end>::type, mpl::plus<mpl::_1, mpl::_1>>::type fir;
//   typedef typename mpl::insert_range<fir,
//           typename mpl::end<fir>::type,
//           typename mpl::erase<T, iter_begin, iter_mid>::type>::type type;
// };

// 2. iter
// (e - i) > n - [n / 2]
// e.g 1 2 3 4 => (4-2) 4 3; 1 2 3 => (3-1) 3 
template <class Out, class It, class End, class Half, class Size, class F1, class F2>
struct iter_impl {
  typedef typename mpl::eval_if<
    typename mpl::greater<typename mpl::distance<It, End>::type, typename mpl::minus<Size, Half>::type>::type,
             typename mpl::apply<F1, typename mpl::deref<It>::type>,
             typename mpl::apply<F2, typename mpl::deref<It>::type>
    >::type X;
  typedef typename iter_impl<typename mpl::push_back<Out, X>::type, typename mpl::next<It>::type, End, Half, Size, F1, F2>::type type;
};

// spec(for iter out)
template <class Out, class End, class Half, class Size, class F1, class F2>
struct iter_impl<Out, End, End, Half, Size, F1, F2> : Out {};

template <class T>
struct double_first_half :
  iter_impl<typename mpl::clear<T>::type,
            typename mpl::begin<T>::type,
            typename mpl::end<T>::type,
            typename mpl::divides<typename mpl::size<T>::type, mpl::int_<2>>::type,
            typename mpl::size<T>::type,
            mpl::plus<mpl::_1, mpl::_1>,
            mpl::identity<mpl::_>> {};

int main(int argc, const char *argv[])
{
  typedef mpl::vector_c<int, 1, 2, 3, 4> v1;
  typedef mpl::vector_c<int, 2, 4, 3, 4> v2;

  static_assert(mpl::equal<double_first_half<v1>::type, v2>::type::value, "equal");
  return 0;
}
