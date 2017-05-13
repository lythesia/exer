#include <boost/mpl/iterator_tags.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/at.hpp>

#include <iostream>
#include <typeinfo>

/*
 * ver1: at_c goes reversely ..
 */
#if 0
template <class D>
struct dimensions_iterator; // fwd decl

template <class T>
struct dimensions {
  static const int size = 0;
  static const int value = 0;
};

template <class T, int N>
struct dimensions<T[N]> {
  static const int size = dimensions<T>::size + 1;
  static const int value = N;

  typedef dimensions_iterator<dimensions<T[N]>> begin;
  typedef typename boost::mpl::advance<begin, boost::mpl::int_<size>>::type end; // utilized size here ..
};

template <class D> // D = T | T[N] | ..
struct dimensions_iterator {
  typedef boost::mpl::forward_iterator_tag category;
};

template <class T>
struct dimensions_iterator<dimensions<T>> {
  typedef boost::mpl::void_ next;
  typedef dimensions<T> type;
};

template <class T, int N>
struct dimensions_iterator<dimensions<T[N]>> {
  typedef dimensions_iterator<dimensions<T>> next;
  typedef dimensions<T[N]> type;
};
#endif

/*
 * ver2: ok
 */
template <class T>
struct dimensions_size {
  static const int value = 0;
};

template <class T, int N>
struct dimensions_size<T[N]> {
  static const int value = dimensions_size<T>::value + 1;
};

template <class T, int N>
struct nth;

template <class T, int Size, int N>
struct nth<T[Size], N> : boost::mpl::eval_if<
      boost::mpl::bool_< N >= dimensions_size<T>::value >,
      boost::mpl::int_<Size>,
      nth<T, N>
    > {};

// nth<int[2][3], 0>: T = int[3], Size = 2, N = 0
//  => nth<int[3], 0>: T = int, Size = 3, N = 0 => int_<3>
// point: we peel of dimensions until conditon `left arr[]::size == n-th dim we want` met (since template deduce goes this direction: int[1][2][3] => 1:int[2][3] => 1:2:int[3])

template <class T, int Nth>
struct dimensions_iterator;

template <class T>
struct dimensions {
  typedef boost::mpl::void_ begin;
  typedef boost::mpl::void_ end;
};

template <class T, int Size>
struct dimensions<T[Size]> {
  typedef dimensions_iterator<T[Size], 0> begin;
  typedef typename boost::mpl::advance<begin, boost::mpl::int_<dimensions_size<T[Size]>::value>>::type end;
};

template <class T, int Nth>
struct dimensions_iterator {
  typedef boost::mpl::forward_iterator_tag category;
};

template <class T, int Size, int Nth>
struct dimensions_iterator<T[Size], Nth> {
  // how `next`? we should utilize `nth`
  typedef dimensions_iterator<T[Size], Nth + 1> next;
  typedef typename nth<T[Size], Nth>::type type;
};

int main(int argc, const char *argv[])
{
  using namespace boost;
  using namespace std;

  typedef int a[10][5][2];
  typedef dimensions<a> seq;
#if 0
  typedef mpl::begin<seq>::type it;
  typedef mpl::next<it>::type it2;
  typedef mpl::next<it2>::type it3;
  typedef mpl::end<seq>::type ed;
  static_assert(seq::size == 2, "size = 2");
  static_assert(mpl::distance<it, it2>::value == 1, "1");
  static_assert(mpl::distance<it, ed>::value == 2, "2");
  typedef mpl::distance<it, ed> d;
  cout << "ed: " << typeid(ed).name() << endl; // err!
  cout << typeid(d).name() << endl; // err!

  typedef mpl::advance<it, mpl::int_<1>>::type itt;
  cout << typeid(itt).name() << endl;
  typedef mpl::distance<it, itt> dd;
  cout << typeid(dd).name() << endl;

  typedef mpl::advance<it, mpl::int_<2>>::type ittt;
  typedef mpl::advance<itt, mpl::int_<1>>::type ittt2;
  cout << "ittt: " << typeid(ittt).name() << endl;
  cout << "ittt2: " << typeid(ittt2).name() << endl;
  typedef mpl::distance<it, ittt> ddd;
  typedef mpl::distance<it, ittt2> ddd2;
  cout << typeid(ddd).name() << endl;
  cout << typeid(ddd2).name() << endl;
  static_assert(ddd::value == 2, "2");
  static_assert(ddd2::value == 2, "2");
  static_assert(mpl::size<seq>::value == 2, "size = 2");

  cout << "====" << endl;
  cout << typeid(it).name() << endl;
  cout << typeid(it2).name() << endl;
  cout << typeid(it3).name() << endl;
#endif

#if 0
  typedef mpl::begin<seq>::type it1;
  // typedef mpl::advance<it1, mpl::int_<0>>::type it2;
  typedef mpl::next<it1>::type it2;
  cout << "begin" << typeid(it1).name() << endl;
  cout << "begin + 1" << typeid(it2).name() << endl;
  cout << "begin::type" << typeid(it1::type).name() << endl;
  cout << "begin + 1::type" << typeid(it2::type).name() << endl;
  cout << "begin::value: " << it1::type::type::value << endl;
  cout << "begin+1::value: " << it2::type::type::value << endl;
  typedef mpl::end<seq>::type ed;
  cout << "distance: " << mpl::distance<it1, ed>::value << endl;
  cout << "size: " << mpl::size<seq>::value << endl;
#endif

#if 0
  cout << "====" << endl;
  typedef mpl::at_c<seq, 0> it1; // iterator
  typedef mpl::begin<seq>::type it2;
  typedef mpl::advance<it2, mpl::int_<0>>::type it3;
  cout << typeid(it1).name() << endl;
  cout << typeid(it1::type).name() << endl;
  cout << typeid(it2).name() << endl;
  cout << typeid(it3).name() << endl;
  cout << "it1::type::value: " << it1::type::value << endl;
  static_assert(it1::type::value == 2, "2");
#endif

#define STATIC_ASSERT(x) static_assert(x, #x)

  STATIC_ASSERT((mpl::size<seq>::value == 3));
  STATIC_ASSERT((mpl::at_c<seq, 0>::type::value == 2));
  STATIC_ASSERT((mpl::at_c<seq, 1>::type::value == 5));
  STATIC_ASSERT((mpl::at_c<seq, 2>::type::value == 10));
  return 0;
}
