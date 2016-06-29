template <unsigned long N>
struct binary {
  static_assert(N%10 <= 1, "not binary");
  static unsigned const value = binary<N/10>::value * 2 + N%10;
};

template <>
struct binary<0> {
  static unsigned const value = 0;
};

int main(int argc, const char *argv[])
{
  // binary<123> notb; // error
  static_assert(binary<1111>::value == 15, "0b1111 = 15");
  return 0;
}
