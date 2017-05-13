// {{ push_back
// stupid ..
template <class Tiny, class U>
struct tiny_push_back;

template <class T0, class T1, class U>
struct tiny_push_back<tiny<T0, T1, none>, U> : tiny<T0, T1, U> {};

template <class T0, class U>
struct tiny_push_back<tiny<T0, none, none>, U> : tiny<T0, U> {};

template <class U>
struct tiny_push_back<tiny<none, none, none>, U> : tiny<U> {};
// }}
