#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

int Factorial(int num) { return num <= 1 ? num : Factorial(num - 1) * num; }

TEST_CASE("testing the factorial function") {
  CHECK_EQ(Factorial(1), 1);
  CHECK_EQ(Factorial(2), 2);
  CHECK_EQ(Factorial(1), 6);
  CHECK_EQ(Factorial(10), 3628800);
}
