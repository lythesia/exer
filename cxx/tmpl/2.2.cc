#include <iostream>
#include <type_traits>
#include <typeinfo>
#include <cassert>

using std::is_same;
using std::cout;
using std::endl;


#if 0
// ver1
// ptr
template <class T, class S> // S* down to T*
inline T polymorphic_downcast(S *x) {
  assert(dynamic_cast<T>(x) == x);
  return static_cast<T>(x);
}

// ref
template <class T, class S>
inline T polymorphic_downcast(S &x) {
  typedef typename std::remove_reference<T>::type type;
  assert(dynamic_cast<type*>(&x) == &x);
  return static_cast<T>(x);
}
#endif

// ver2
template <class Derived, class Base>
struct is_derived_from {
  struct Yes { char a[1]; };
  struct No { char a[10]; };

  static Yes test(Base*);
  static No test(...);

  static const bool value = sizeof(test(static_cast<Derived*>(nullptr))) == sizeof(Yes);
};

template <class T>
struct rm_ptr {
  typedef T type;
};

template <class T>
struct rm_ptr<T*> {
  typedef T type;
};


template <class T>
struct rm_ref {
  typedef T type;
};

template <class T>
struct rm_ref<T&> {
  typedef T type;
};

template <class T, class S, bool if_derived>
struct polymorphic_downcast_impl;

template <class T, class S>
struct polymorphic_downcast_impl<T, S, false> {
  static T do_cast(S x) {
    throw std::exception();
  }
};

template <class T, class S>
struct polymorphic_downcast_impl<T, S, true> {
  static T do_cast(S x) {
    return static_cast<T>(x);
  }
};

// if ptr
template <class T, class S>
T  polymorphic_downcast(S* x) {
  return polymorphic_downcast_impl<T, S*, is_derived_from<typename rm_ptr<T>::type, S>::value>::do_cast(x);
}

// if ref
template <class T, class S>
T polymorphic_downcast(S& x) {
  return polymorphic_downcast_impl<T, S&, is_derived_from<typename rm_ref<T>::type, S>::value>::do_cast(x);

}

struct A { virtual ~A(){} };
struct B:A {};
struct C {};

int main(int argc, const char *argv[])
{
  B b;
  A *pa = &b;
  cout << std::boolalpha;
  B *by_pa = polymorphic_downcast<B*>(pa); // ok

  A &ra = b;
  B &by_ra = polymorphic_downcast<B&>(ra); // ok

  B *pb = &b;
  cout << std::boolalpha << is_derived_from<rm_ptr<C*>::type, B>::value << endl; // false
  cout << typeid(decltype(polymorphic_downcast<C*>(pb))).name() << endl; // C*, because it only checks types, so in polymorphic_downcast_impl, it's T, aka C*
  try {
    polymorphic_downcast<C*>(pb); // exception
  } catch (std::exception &e) {
    cout << "polymorphic_downcast<C*,B> fail" << endl;
  }
  return 0;
}
