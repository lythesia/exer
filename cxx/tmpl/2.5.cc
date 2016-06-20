#include <iostream>
#include <type_traits>
using namespace std;

template <class T>
struct return_type {
  typedef void type;
};

template <class T>
struct return_type<T()> {
  typedef T type;
};

template <typename T>
struct is_pointer_to_const {
  static const bool value = std::is_pointer<T>::value && std::is_const<typename std::remove_pointer<T>::type>::value;
};

template <class T>
struct type_descriptor {
  static string info() {
    if(is_const<T>::value) {
      return "const " + type_descriptor<typename remove_const<T>::type>::info();
    } else if(is_volatile<T>::value) {
      return "volatile " + type_descriptor<typename remove_volatile<T>::type>::info();
    } else if(is_pointer<T>::value) {
      return "pointer to " + type_descriptor<typename remove_pointer<T>::type>::info();
    } else if(is_reference<T>::value) {
      return "reference to " + type_descriptor<typename remove_reference<T>::type>::info();
    } else if(is_pointer_to_const<T>::value) {
      return "const " + type_descriptor<typename remove_const<typename remove_pointer<T>::type>::type>::info();
    } else if(is_array<T>::value) {
      return "array of " + type_descriptor<typename remove_extent<T>::type>::info();
    } else if(is_function<T>::value){
      return "function returing " + type_descriptor<typename return_type<T>::type>::info();
    } else if(is_same<T, char>::value) {
      return "char";
    } else if(is_same<T, short>::value) {
      return "short";
    } else if(is_same<T, int>::value) {
      return "int";
    } else if(is_same<T, long>::value) {
      return "long";
    } else {
      return "";
    }
  }
};

template <class T>
std::ostream & operator<<(std::ostream &os, const type_descriptor<T> &desc) {
  return os << type_descriptor<T>::info();
}

int main(int argc, const char *argv[])
{
  cout << type_descriptor<const int * const>() << endl;
  cout << type_descriptor<char *(*[])()>() << endl;
  return 0;
}
