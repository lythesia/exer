#include <boost/mpl/or.hpp>
#include <boost/mpl/and.hpp>
#include <boost/type_traits/is_integral.hpp>

namespace mpl = boost::mpl;

template <class X> struct fail_on_inst;

int main(int argc, const char *argv[])
{
  static_assert(mpl::or_<boost::is_integral<int>, fail_on_inst<int>>::value, "true");
  static_assert(mpl::or_<boost::is_integral<void>, fail_on_inst<int>>::value, "?"); // error: implicit instantiation of undefined template 'fail_on_inst<int>'

  static_assert(!mpl::and_<boost::is_integral<void>, fail_on_inst<int>>::value, "false");
  static_assert(mpl::and_<boost::is_integral<int>, fail_on_inst<int>>::value, "?"); // error: implicit instantiation of undefined template 'fail_on_inst<int>'

  return 0;
}
