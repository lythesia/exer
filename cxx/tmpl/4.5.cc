#include <boost/type_traits.hpp>
#include <boost/mpl/if.hpp>
#include <vector>
#include <algorithm>

namespace mpl = boost::mpl;

template <class C>
struct iter_traits : mpl::if_<boost::is_const<C>, typename C::const_iterator, typename C::iterator> {};

template <class C, class V>
typename iter_traits<C>::type
container_find(C& c, const V &v) {
  return std::find(c.begin(), c.end(), v);
}

int main(int argc, const char *argv[])
{
  typedef std::vector<int> vi;
  vi v = {1,2,3};
  const vi cv = {4,5,6};

  static_assert(boost::is_same<vi::iterator, decltype(container_find(v, 1))>::value, "iter");
  static_assert(boost::is_same<vi::const_iterator, decltype(container_find(cv, 4))>::value, "const iter");
  return 0;
}
