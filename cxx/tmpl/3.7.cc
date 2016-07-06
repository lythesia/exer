#include <iostream>
#include <vector>
#include <type_traits>
#include <typeinfo>
#include <boost/mpl/apply.hpp>
#include <boost/mpl/lambda.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/plus.hpp>
namespace mpl = boost::mpl;
using mpl::_1;
using mpl::_2;
using std::cout;
using std::endl;
using std::is_same;


int main(int argc, const char *argv[])
{
  /* 1
   * typedef mpl::lambda<mpl::lambda<_1>>::type t1 ;
   */
  // by step
  typedef mpl::lambda<_1> ident; // placeholder exp: which is identical mapper, e.g. give int return int, or gives meta-func-class if placeholder exp
  typedef mpl::lambda<ident>::type ident_meta_func_class; // turns it(given type) into meta-func-class, or if it alread is
  static_assert(is_same<ident_meta_func_class::apply<int>::type, int>::value, "identical"); // equals mpl::apply<ident_meta_func_class, int>::type

  /* 2 
   * typedef mpl::apply<_1, mpl::plus<_1, _2>>::type t2;
   */
  // by step
  typedef mpl::plus<_1, _2 > adder; // placeholder exp: given two addable type, given sum type
  typedef mpl::apply<_1, adder>::type adder_again; // since it evals into _1::apply<adder> == adder, according to _1 's definition
  static_assert(is_same<adder, adder_again>::value, "adder");
  static_assert(mpl::apply<adder_again, mpl::int_<1>, mpl::int_<2>>::type::value == 3, "3");

  /* 3
   * typedef mpl::apply<_1, std::vector<int>>::type t3;
   */
  // same as above, so ..
  typedef std::vector<int> vi;
  static_assert(is_same<mpl::apply<_1, vi>::type, vi>::value, "vector<int>");

  /* 4
   * typedef mpl::apply<_1, std::vector<_1>>::type t4;
   */
  typedef std::vector<_1> vec_T; // placeholder exp: given T, get vector<T>
  typedef mpl::apply<_1, vec_T>::type vec_T_again;
  static_assert(is_same<vec_T, vec_T_again>::value, "vec_T");
  static_assert(is_same<mpl::apply<vec_T_again, int>::type, vi>::value, "vector<int> from vec_T");

  /* 5
   * typedef mpl::apply<mpl::lambda<_1>, std::vector<int>>::type t5;
   */
  typedef mpl::apply<ident, vi>::type vi_again; // ident::apply<vi>::type which givens vi as ident defines
  static_assert(is_same<vi_again, vi>::value, "vi_again");

  /* 6
   * typedef mpl::apply<mpl::lambda<_1>, std::vector<_1>>::type t6;
   */
  typedef mpl::apply<ident, vec_T>::type vec_T_meta_func_class;
  static_assert(is_same<mpl::apply<vec_T_meta_func_class, int>::type, vi>::value, "vector<int> from vec_T");

  /* 7
   * typedef mpl::apply<mpl::lambda<_1>, mpl::plus<_1, _2>>::type t7;
   */
  typedef mpl::apply<ident, adder>::type adder_meta_func_class;
  static_assert(mpl::apply<adder_meta_func_class, mpl::int_<1>, mpl::int_<2>>::type::value == 3, "3");

  /* 8
   * typedef mpl::apply<_1, mpl::lambda<mpl::plus<_1, _2>>>::type t8;
   */
  typedef mpl::lambda<adder> adder_lambda; // if we get ::type, it is adder meta-func-class
  typedef mpl::apply<_1, adder_lambda>::type adder_meta_func_class_again; // so when applying, it is same effect
  static_assert(mpl::apply<adder_meta_func_class_again, mpl::int_<1>, mpl::int_<2>>::type::value == 3, "3");

  return 0;
}
