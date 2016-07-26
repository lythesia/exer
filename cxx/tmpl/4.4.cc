#include <boost/type_traits.hpp>
#include <boost/mpl/not.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/or.hpp>

namespace mpl = boost::mpl;

// base
// template< class T >
// struct is_member_pointer_helper         : std::false_type {};
 
// template< class T, class U >
// struct is_member_pointer_helper<T U::*> : std::true_type {};
 
// template< class T >
// struct is_member_pointer : 
//     is_member_pointer_helper<typename std::remove_cv<T>::type> {};

template <class T>
struct is_data_member_pointer : mpl::and_<boost::is_member_pointer<T>, mpl::not_<boost::is_member_function_pointer<T>>> {};

template <class T>
struct is_pointer_to_function : mpl::and_<boost::is_pointer<T>, boost::is_function<typename boost::remove_pointer<T>::type>> {};

template <class T>
struct is_reference_to_function_pointer : mpl::and_<boost::is_reference<T>, is_pointer_to_function<typename boost::remove_reference<T>::type>> {};

template <class T>
struct is_reference_to_non_const : mpl::and_<boost::is_reference<T>, mpl::not_<boost::is_const<T>>> {};

struct A {
  int *p;
};

int main(int argc, const char *argv[])
{
  static_assert(is_data_member_pointer<int A::*>(), "mem_ptr");

  static_assert(is_pointer_to_function<int (*)()>(), "fun_ptr");

  typedef int (*pf)();
  typedef pf &ref_to_pf;
  static_assert(is_reference_to_function_pointer<ref_to_pf>(), "ref_to_fun_ptr");

  static_assert(is_reference_to_non_const<ref_to_pf>(), "ref_to_non_const");
  return 0;
}
