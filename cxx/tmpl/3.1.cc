#include <boost/mpl/vector_c.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/mpl/plus.hpp>
#include <boost/mpl/equal.hpp>
using namespace boost;

struct inc {
  template <class T>
  struct apply : mpl::plus<T, mpl::int_<1>> {};
};

int main(int argc, const char *argv[])
{
  typedef mpl::vector_c<int, 1, 2, 3> vec_123; 
  typedef mpl::vector_c<int, 2, 3, 4> vec_234; 
  typedef mpl::transform<vec_123, inc>::type ans;
  static_assert(mpl::equal<ans, vec_234>::type::value, "equal");
  return 0;
}
