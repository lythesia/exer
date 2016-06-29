#include <boost/mpl/vector_c.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/mpl/multiplies.hpp>
#include <boost/mpl/equal.hpp>
using namespace boost;

struct sq {
  template <class T>
  struct apply : mpl::multiplies<T, T> {};
};

int main(int argc, const char *argv[])
{
  typedef mpl::vector_c<int, 1, 2, 3> vec_123;
  typedef mpl::vector_c<int, 1, 4, 9> vec_149;
  typedef mpl::transform<vec_123, sq>::type ans;
  static_assert(mpl::equal<ans, vec_149>::type::value, "equal");
  return 0;
}
