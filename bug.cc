#include <functional>
#include <iostream>

std::function<int()> f(int const &i)
{
  return [=]() { return i; };
}

int main()
{
  int x = 42;
  std::cout << f(x)();  // uninitialized value
}
