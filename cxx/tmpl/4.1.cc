#include <boost/type_traits/is_integral.hpp>

namespace mpl = boost::mpl;

/* or */
template <class T1, bool c, class T2> struct logical_or_impl;

template <class T1, class T2>
struct logical_or {
  static const bool value = logical_or_impl<T1, T1::value, T2>::value;
};

template <class T1, bool c, class T2>
struct logical_or_impl {
  static const bool value = c;
};

template <class T1, class T2>
struct logical_or_impl<T1, false, T2> {
  static const bool value = T2::value;
};

/* and */

template <class X> struct fail_on_inst;

template <class T1, bool c, class T2> struct logical_and_impl;

template <class T1, class T2>
struct logical_and {
  static const bool value = logical_and_impl<T1, T1::value, T2>::value;
};

template <class T1, bool c, class T2>
struct logical_and_impl {
  static const bool value = c;
};

template <class T1, class T2>
struct logical_and_impl<T1, true, T2> {
  static const bool value = T2::value;
};

int main(int argc, const char *argv[])
{
  static_assert(logical_or<boost::is_integral<int>, fail_on_inst<int>>::value, "true");
  // static_assert(logical_or<boost::is_integral<void>, fail_on_inst<int>>::value, "?"); // error

  static_assert(!logical_and<boost::is_integral<void>, fail_on_inst<int>>::value, "false");
  // static_assert(logical_and<boost::is_integral<int>, fail_on_inst<int>>::value, "?"); // error
  return 0;
}
