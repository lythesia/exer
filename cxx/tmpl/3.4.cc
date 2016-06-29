#include <type_traits>
#include <boost/mpl/apply.hpp>
#include <boost/mpl/lambda.hpp>
#include <boost/mpl/placeholders.hpp>
using namespace std;
namespace mpl = boost::mpl;
using mpl::_;

template <class F, class X>
struct twice : mpl::apply<F, typename mpl::apply<F,X>::type> {};

int main(int argc, const char *argv[])
{
  static_assert(is_same<int****,
                        twice<
                          twice<
                            mpl::lambda<add_pointer<_>>::type,
                             _>, 
                          int>::type>::value, "int****"); 
  return 0;
}

// need think about it..
