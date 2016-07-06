#include <type_traits>
#include <boost/mpl/equal.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/mpl/lambda.hpp>
#include <boost/mpl/placeholders.hpp>
namespace mpl = boost::mpl;
using mpl::_;

// just wrap meta-func into meta-func-class
struct lambda_twice {
  template <class F, class X>
    struct apply : mpl::apply<F, typename mpl::apply<F, X>::type> {};
};

int main(int argc, const char *argv[])
{
  typedef mpl::lambda<std::add_pointer<_>>::type lambda_add_ptr;
  static_assert(mpl::equal<
      mpl::apply<lambda_twice, lambda_add_ptr, int>::type,
      int**
      >::type::value, "int**");
  return 0;
}
