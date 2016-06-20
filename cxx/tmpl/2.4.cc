#include <iostream>
#include <string>

template <class T>
struct type_descriptor {};

template <>
struct type_descriptor<char> {
  static std::string info() {
    return "char";
  }
};

template <>
struct type_descriptor<short> {
  static std::string info() {
    return "short";
  }
};

template <>
struct type_descriptor<int> {
  static std::string info() {
    return "int";
  }
};

template <>
struct type_descriptor<long> {
  static std::string info() {
    return "long";
  }
};

template <class T>
struct type_descriptor<T*> {
  static std::string info() {
    return type_descriptor<T>::info() + "*";
  }
};

template <class T>
struct type_descriptor<T&> {
  static std::string info() {
    return type_descriptor<T>::info() + "&";
  }
};

template <class T>
struct type_descriptor<const T> {
  static std::string info() {
    return "const " + type_descriptor<T>::info();
  }
};

template <class T>
struct type_descriptor<const volatile T> {
  static std::string info() {
    return "const volatile " + type_descriptor<T>::info();
  }
};

template <class T>
struct type_descriptor<T* const> {
  static std::string info() {
    return type_descriptor<T>::info() + "* const";
  }
};

template <class T>
std::ostream & operator<<(std::ostream &os, const type_descriptor<T> &desc) {
  return os << type_descriptor<T>::info();
}

int main(int argc, const char *argv[])
{
  using std::cout;
  using std::endl;

  cout << type_descriptor<int>() << endl;
  cout << type_descriptor<char*>() << endl;
  cout << type_descriptor<long const * &>() << endl;
  cout << type_descriptor<long * const>() << endl;
  return 0;
}
