#include <iostream>

void f( int const &i )
{
  std::cout << [=](){ return i; }();
}

int main()
{
  int x = 42;
  f(x); // uninitialized value
}
