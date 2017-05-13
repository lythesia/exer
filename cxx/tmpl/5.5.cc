#include <iostream>
#include <typeinfo>
#include <type_traits>
#include <boost/mpl/int.hpp>
#include <boost/mpl/iterator_tags.hpp>
#include <boost/mpl/next_prior.hpp>
#include <boost/mpl/plus.hpp>
#include <boost/mpl/minus.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/mpl/if.hpp>

struct none {};
struct tiny_tag {};

template <class T0 = none, class T1 = none, class T2 = none>
struct tiny {
  typedef tiny_tag tag;
  typedef tiny type;
  typedef T0 t0;
  typedef T1 t1;
  typedef T2 t2;
};

// {{ tiny at
template <class Tiny, int N>
struct tiny_at;

// spec for each index
template <class Tiny>
struct tiny_at<Tiny, 0> {
  typedef typename Tiny::t0 type;
};

template <class Tiny>
struct tiny_at<Tiny, 1> {
  typedef typename Tiny::t1 type;
};

template <class Tiny>
struct tiny_at<Tiny, 2> {
  typedef typename Tiny::t2 type;
};
// if `tiny_at<something >= 3>`, we get CE

template <class T, class Pos>
struct at;

template <class T0, class T1, class T2, class Pos>
struct at<tiny<T0, T1, T2>, Pos> : tiny_at<tiny<T0, T1, T2>, Pos::value> {}; // always use `::type` to get result!; `Pos` need to be something wrap int-const?
// }}


// {{
// general at
template <class Tag>
struct at_impl;

template <>
struct at_impl<tiny_tag> {
  template <class Tiny, class N> struct apply : tiny_at<Tiny, N::value> {};
};

template <class Tiny, class N>
struct at2 : at_impl<typename Tiny::tag>::template apply<Tiny, N> {};
// }}

// {{
// iterator (sure, iterator should have position info)
template <class Tiny, class Pos>
struct tiny_iterator {
  typedef Tiny container;
  typedef Pos position;
  typedef boost::mpl::random_access_iterator_tag category; // TODO: what use for?
};

// foward
template <class T>
struct next {};

// spec for tiny
template <class Tiny, class Pos>
struct next<tiny_iterator<Tiny, Pos>> {
  typedef tiny_iterator<Tiny, typename boost::mpl::next<Pos>::type> type;
};

// bi-direction
template <class T>
struct prior {};

// spec for tiny
template <class Tiny, class Pos>
struct prior<tiny_iterator<Tiny, Pos>> {
  typedef tiny_iterator<Tiny, typename boost::mpl::prior<Pos>::type> type;
};

// deref
template <class T>
struct deref {};

template <class Tiny, class Pos>
struct deref<tiny_iterator<Tiny, Pos>> : at<Tiny, Pos> {};

// advance
template <class T, class N>
struct advance {};

template <class Tiny, class Pos, class N>
struct advance<tiny_iterator<Tiny, Pos>, N> {
  typedef tiny_iterator<Tiny, typename boost::mpl::plus<Pos, N>::type> type;
};

// distance
// iterator with diff pos ==> diff type!
template <class T1, class T2>
struct distance {};

// TODO: seems we dont need Tiny?
template <class Tiny, class Pos1, class Pos2>
struct distance<
  tiny_iterator<Tiny, Pos1>,
  tiny_iterator<Tiny, Pos2>
> : boost::mpl::minus<Pos2, Pos1> {}; // Pos2 - Pos1
// }}

// {{ begin/end
// begin
template <class Tag>
struct begin_impl {};

// spec for tiny(tag)
template <>
struct begin_impl<tiny_tag> {
  template <class Tiny> // TODO: `Tiny` can we backref this type?
    struct apply {
      typedef tiny_iterator<Tiny, boost::mpl::int_<0>> type;
    };
};

template <class T>
struct begin : boost::mpl::apply<begin_impl<typename T::tag>, T> {}; // TODO: ::type ==> tiny_iterator ?? keep in mind, meta-func always use ::type to get result

// end

// we need size
template <class T0 = none, class T1 = none, class T2 = none>
struct tiny_size : boost::mpl::int_<3> {};

template <class T0, class T1>
struct tiny_size<T0, T1, none> : boost::mpl::int_<2> {};

template <class T0>
struct tiny_size<T0, none, none> : boost::mpl::int_<1> {};

template <>
struct tiny_size<none, none, none> : boost::mpl::int_<0> {};

template <class Tag>
struct size_impl {};

template <>
struct size_impl<tiny_tag> {
  template <class Tiny>
  struct apply : tiny_size<typename Tiny::t0, typename Tiny::t1, typename Tiny::t2> {};
};

template <class T>
struct size : boost::mpl::apply<size_impl<typename T::tag>, T> {};

template <class Tag>
struct end_impl {};

template <>
struct end_impl<tiny_tag> {
  template <class Tiny>
    struct apply {
      typedef tiny_iterator<Tiny, typename size<Tiny>::type> type; // we get iterator
    };
};

template <class T>
struct end : boost::mpl::apply<end_impl<typename T::tag>, T> {};
// }}

// {{ clear
template <class Tag>
struct clear_impl {};

template <>
struct clear_impl<tiny_tag> {
  template <class Tiny>
    struct apply : tiny<> {}; // as old-school fp, we return "new type"
};

template <class T>
struct clear : boost::mpl::apply<clear_impl<typename T::tag>, T> {};
// }}

template <class Tag>
struct push_front_impl {};

// {{ push_front
template <class Tiny, class T, int N>
struct tiny_push_front;

template <class Tiny, class T>
struct tiny_push_front<Tiny, T, 0> : tiny<T> {};

template <class Tiny, class T>
struct tiny_push_front<Tiny, T, 1> : tiny<T, typename Tiny::t0> {};

template <class Tiny, class T>
struct tiny_push_front<Tiny, T, 2> : tiny<T, typename Tiny::t0, typename Tiny::t1> {};

template <class Tag>
struct push_front_impl;

template <>
struct push_front_impl<tiny_tag> {
  template <class Tiny, class T>
    struct apply : tiny_push_front<Tiny, T, size<Tiny>::value> {};
};

template <class T, class U>
struct push_front : boost::mpl::apply<push_front_impl<typename T::tag>, T, U> {};
// }}

// {{ push_back
template <class Tiny, class T, int N>
struct tiny_push_back;

template <class Tiny, class T>
struct tiny_push_back<Tiny, T, 0> : tiny<T> {};

template <class Tiny, class T>
struct tiny_push_back<Tiny, T, 1> : tiny<typename Tiny::t0, T> {};

template <class Tiny, class T>
struct tiny_push_back<Tiny, T, 2> : tiny<typename Tiny::t0, typename Tiny::t1, T> {};

template <class Tag>
struct push_back_impl;

template <>
struct push_back_impl<tiny_tag> {
  template <class Tiny, class T>
    struct apply : tiny_push_back<Tiny, T, size<Tiny>::value> {};
};

template <class T, class U>
struct push_back : boost::mpl::apply<push_back_impl<typename T::tag>, T, U> {};
// }}

// {{ pop_front
template <class Tiny, int N>
struct tiny_pop_front;

template <class Tiny>
struct tiny_pop_front<Tiny, 1>: tiny<> {};

template <class Tiny>
struct tiny_pop_front<Tiny, 2>: tiny<typename Tiny::t1> {};

template <class Tiny>
struct tiny_pop_front<Tiny, 3>: tiny<typename Tiny::t1, typename Tiny::t2> {};

template <class Tag>
struct pop_front_impl;

template <>
struct pop_front_impl<tiny_tag> {
  template <class Tiny>
    struct apply : tiny_pop_front<Tiny, size<Tiny>::value> {};
};

template <class T>
struct pop_front : boost::mpl::apply<pop_front_impl<typename T::tag>, T> {};
// }}

// {{ pop_back
template <class Tiny, int N>
struct tiny_pop_back;

template <class Tiny>
struct tiny_pop_back<Tiny, 1>: tiny<> {};

template <class Tiny>
struct tiny_pop_back<Tiny, 2>: tiny<typename Tiny::t0> {};

template <class Tiny>
struct tiny_pop_back<Tiny, 3>: tiny<typename Tiny::t0, typename Tiny::t1> {};

template <class Tag>
struct pop_back_impl;

template <>
struct pop_back_impl<tiny_tag> {
  template <class Tiny>
    struct apply : tiny_pop_back<Tiny, size<Tiny>::value> {};
};

template <class T>
struct pop_back : boost::mpl::apply<pop_back_impl<typename T::tag>, T> {};
// }}

// {{ erase
template <class Tiny, int start, int len>
struct tiny_erase;

template <class Tiny>
struct tiny_erase<Tiny, 0, 1> : tiny<typename Tiny::t1, typename Tiny::t2> {};

template <class Tiny>
struct tiny_erase<Tiny, 0, 2> : tiny<typename Tiny::t2> {};

template <class Tiny>
struct tiny_erase<Tiny, 0, 3> : tiny<> {};

template <class Tiny>
struct tiny_erase<Tiny, 1, 1> : tiny<typename Tiny::t0, typename Tiny::t2> {};

template <class Tiny>
struct tiny_erase<Tiny, 1, 2> : tiny<typename Tiny::t0> {};

template <class Tiny>
struct tiny_erase<Tiny, 2, 1> : tiny<typename Tiny::t0, typename Tiny::t1> {};

template <class Tag>
struct erase_impl;

template <>
struct erase_impl<tiny_tag> {
  template <class Tiny, class start, class len>
    struct apply : tiny_erase<Tiny, start::type::value, len::type::value> {}; // TODO: we can ::type::value without typename, but how to write distance<..>::type::value?
};

template <class T, class First, class Last>
struct erase : boost::mpl::apply<erase_impl<typename T::tag>,
                                 T,
                                 distance<typename begin<T>::type, First>, // TODO: mpl::apply requires all type? int etc types not allowed?? @apply: template argument for template type parameter must be a type
                                                                           // check this thread: http://stackoverflow.com/questions/19435121/boostmplapply-only-works-with-typed-template-args
                                 distance<First, Last>> {};
// }}


int main(int argc, const char *argv[])
{
  using std::is_same;
  typedef tiny<int, char> ic;
  typedef tiny<char, int> ci;
  typedef tiny<int> i;
  typedef tiny<char> c;
  typedef tiny<> nil;

  static_assert(is_same<i, pop_front<ci>::type>::value, "pop_front");
  static_assert(is_same<c, pop_back<ci>::type>::value, "pop_back");
  static_assert(is_same<i, erase<ic, next<begin<ic>::type>::type, end<ic>::type>::type>::value, "erase");
  static_assert(is_same<nil, erase<ic, begin<ic>::type, end<ic>::type>::type>::value, "erase");

  return 0;
}
