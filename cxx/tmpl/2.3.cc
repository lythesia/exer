#include <iostream>
#include <type_traits>
using namespace std;

template <class T>
struct type_descriptor {
  
  static string info() {
    if(is_const<T>::value) {
      return "const " + type_descriptor<typename remove_const<T>::type>::info();
    } else if(is_pointer<T>::value) {
      return type_descriptor<typename remove_pointer<T>::type>::info() + "*";
    } else if(is_reference<T>::value) {
      return type_descriptor<typename remove_reference<T>::type>::info() + "&";
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
ostream & operator<<(ostream &os, type_descriptor<T> desc) {
  return os << type_descriptor<T>::info();
}

int main(int argc, const char *argv[])
{
  cout << type_descriptor<int>() << endl;
  cout << type_descriptor<char*>() << endl;
  cout << type_descriptor<long const * &>() << endl;
  return 0;
}
