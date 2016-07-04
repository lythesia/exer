#include <boost/mpl/vector_c.hpp>
#include <boost/mpl/lambda.hpp>
#include <boost/mpl/equal.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/plus.hpp>
#include <boost/mpl/transform.hpp>
#include <iostream>

using namespace boost;
using mpl::_;

typedef mpl::vector_c<int,1,0,0> mass;
typedef mpl::vector_c<int,0,1,0> length;
// typedef mpl::vector_c<int,0,0,1> time;
typedef mpl::vector_c<int,0,1,-1> velocity;
typedef mpl::vector_c<int,0,1,-2> acceleration;
typedef mpl::vector_c<int,1,1,-2> force;

template <class T, class D>
struct quantity {
  // exp ctor
  explicit quantity(T x) : _value(x) {}
  // getter
  T value() const { return _value; }
  // cast
  template <class OtherD>
    quantity(const quantity<T,OtherD> &rhs) : _value(rhs.value()) {
      static_assert(mpl::equal<D, OtherD>::type::value, "D != OtherD");
    }
  // assign
  template <class OtherD>
    quantity<T,D> &operator=(const quantity<T,OtherD> &rhs) {
      static_assert(mpl::equal<D, OtherD>::type::value, "D != OtherD");
      _value = rhs.value();
      return *this;
    }
private:
  T _value;
};

template <class T, class D, class OtherD>
quantity<T,D> operator+(quantity<T,D> x, quantity<T,OtherD> y) {
  static_assert(mpl::equal<D, OtherD>::type::value, "D != OtherD");
  return quantity<T,D>(x.value() + y.value());
}

template <class T, class D1, class D2>
quantity<T, typename mpl::transform<D1, D2, mpl::plus<_,_>>::type>
operator*(quantity<T,D1> x, quantity<T,D2> y) {
  typedef typename mpl::transform<D1, D2, mpl::plus<_,_>>::type dim;
  return quantity<T,dim>(x.value() * y.value());
}

int main(int argc, const char *argv[])
{
  using std::cout;
  using std::endl;
  quantity<float, force> f(1.0f);
  quantity<float, mass> m(0.5f);
  quantity<float, acceleration> a(2.0f);
  // quantity<float, force> ff = m*a; // ok
  f = m*a;
  cout << f.value() << endl;

  f = f + m*a;
  cout << f.value() << endl;

  f = m*a + f;
  cout << f.value() << endl;
  // f = f + m*a;
  return 0;
}
