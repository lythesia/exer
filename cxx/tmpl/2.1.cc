#include <iostream>
#include <type_traits>
#include <typeinfo>

template <class C, class X, class Y>
struct replace_type;

template <typename C, typename X, typename Y, bool>
struct replace_type_impl;


template <typename C, typename X, typename Y>
struct replace_type_impl<C, X, Y, true> {
  typedef Y type;
};

template <typename C, typename X, typename Y>
struct replace_type_impl<C, X, Y, false> {
  typedef X type;
};

template <typename C, typename X, typename Y>
struct replace_type {
#pragma message("begin main")
  typedef typename replace_type_impl<C, X, Y, std::is_same<C, X>::value>::type type;
#pragma message("end main")
};

// spec: ptr
template <typename C, typename X, typename Y>
struct replace_type_impl<C*, X, Y, false> {
#pragma message("begin pointer")
  typedef typename replace_type<C, X, Y>::type *type;
#pragma message("end pointer")
};

// spec: const
template <typename C, typename X, typename Y>
struct replace_type_impl<C const, X, Y, false> {
  typedef typename replace_type<C, X, Y>::type const type;
};

// spec: ref
template <typename C, typename X, typename Y>
struct replace_type_impl<C&, X, Y, false> {
  typedef typename replace_type<C, X, Y>::type &type;
};

// spec: array
template <typename C, typename X, typename Y>
struct replace_type_impl<C[], X, Y, false> {
  typedef typename replace_type<C, X, Y>::type type[];
};

// spec: array with sz
template <typename C, typename X, typename Y, size_t N>
struct replace_type_impl<C[N], X, Y, false> {
#pragma message("begin array with size")
  typedef typename replace_type<C, X, Y>::type type[N];
#pragma message("end array with size")
};

// spec: fun
template <typename X, typename Y, typename A>
struct replace_type_impl<A(), X, Y, false> {
  typedef typename replace_type<A, X, Y>::type __return_type;
  typedef __return_type type();
};

// spec: fun
template <typename X, typename Y, typename A, typename B>
struct replace_type_impl<A(B), X, Y, false> {
  typedef typename replace_type<A, X, Y>::type __return_type;
  typedef typename replace_type<B, X, Y>::type __param_type;
  typedef __return_type type(__param_type);
};

// spec: fun
template <typename X, typename Y, typename A, typename B, typename C>
struct replace_type_impl<A(B, C), X, Y, false> {
  typedef typename replace_type<A, X, Y>::type __return_type;
  typedef typename replace_type<B, X, Y>::type __param1_type;
  typedef typename replace_type<C, X, Y>::type __param2_type;
  typedef __return_type type(__param1_type, __param2_type);
};
// what if more than two args?

int main(int argc, const char *argv[])
{
  using std::is_same;
  using std::cout;
  using std::endl;

  cout << std::boolalpha;
  // cout << is_same<int*, replace_type<void*, void, int>::type>::value << endl;

  cout << typeid(replace_type<int const*[10], int const, long>::type).name() << endl; // long* [10]
  // cout << is_same<long*[10], replace_type<int const*[10], int const, long>::type>::value << endl;

  cout << typeid(replace_type<int(int,int), int, long>::type).name() << endl;
  cout << typeid(replace_type<int(int,int,int), int, long>::type).name() << endl; // not work
  // cout << is_same<long& (*)(long&), replace_type<char& (*)(char&), char&, long&>::type>::value << endl;
  return 0;
}
