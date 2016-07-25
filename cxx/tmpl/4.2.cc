#include <boost/type_traits/is_integral.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/eval_if.hpp>

namespace mpl = boost::mpl;

typedef mpl::true_ true_type;
typedef mpl::false_ false_type;

/* raw version */
// fwd decl
// template <bool c, class T2, class T3, class T4, class T5> struct logical_or_impl;

// template <class T1, class T2, class T3 = false_type, class T4 = false_type, class T5 = false_type>
// struct logical_or : logical_or_impl<T1::value, T2, T3, T4, T5> {};

// template <>
// struct logical_or<false_type,false_type,false_type,false_type,false_type> : false_type {};

// impl
// template <bool c, class T2, class T3, class T4, class T5>
// struct logical_or_impl : true_type {};

// false spec
// template <class T2, class T3, class T4, class T5>
// struct logical_or_impl<false, T2, T3, T4, T5> : logical_or<T2, T3, T4, T5, false_type> {}; // post-set false_type to short-cut OR: if false, advance

/* elegant version */
template <class T1, class T2, class T3 = false_type, class T4 = false_type, class T5 = false_type>
struct logical_or : mpl::eval_if<T1, true_type, logical_or<T2, T3, T4, T5, false_type>> {};

template <>
struct logical_or<false_type,false_type,false_type,false_type,false_type> : false_type {};

template <class T1, class T2, class T3 = true_type, class T4 = true_type, class T5 = true_type>
struct logical_and : mpl::eval_if<T1, logical_and<T2, T3, T4, T5, true_type>, false_type> {};

template <>
struct logical_and<true_type,true_type,true_type,true_type,true_type> : true_type {};

// probe
template <class X> struct fail_on_inst;

int main(int argc, const char *argv[])
{
  static_assert(
      logical_or<
        boost::is_integral<int>,
        boost::is_integral<bool>,
        boost::is_integral<char>,
        fail_on_inst<int>
      >::type::value, "true");

  static_assert(
      !logical_and<
        boost::is_integral<void>,
        boost::is_integral<bool>,
        boost::is_integral<char>,
        fail_on_inst<int>
      >::type::value, "false");
  return 0;
}
