#include <boost/mpl/plus.hpp>
#include <boost/mpl/minus.hpp>
#include <boost/mpl/multiplies.hpp>
#include <boost/mpl/lambda.hpp>
#include <boost/mpl/apply.hpp>

int main(int argc, const char *argv[])
{
  using namespace boost::mpl;

  typedef plus<_1, int_<42>> add42; // partial
  static_assert(apply<add42, int_<2>>::type::value == 44, "44");

  typedef multiplies<plus<_1, _2>, minus<_1, _2>> sq_diff; // compose: a^2 - b^2
  static_assert(apply<sq_diff, int_<5>, int_<4>>::type::value == 9, "9");
  return 0;
}

// placeholder is template class, so it's type T, thus it can be apply into template parameter
// when tempalte eval, ::apply(though it's also struct, a type) inside placeholder class will
// be called, and N-th arg will be returned and replace that template parameter
