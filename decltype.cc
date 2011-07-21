// decltype で指定される型

#include "get_typename.hpp"

template< class T >
struct type {};

#include <iostream>

#define PRINT_DECLTYPE( expr )  \
  ( std::cout << #expr << " : " << pezzi::get_typename<decltype(expr)>() << std::endl )

void f();

int main()
{
  auto x = 0;
  PRINT_DECLTYPE( x );
  PRINT_DECLTYPE( (x) );
  
  auto const& y = x;
  PRINT_DECLTYPE( y );
  PRINT_DECLTYPE( (y) );
  
  PRINT_DECLTYPE( f );
  PRINT_DECLTYPE( (f) );
  
  struct X {
    int z;
    
    void foo() const {
      PRINT_DECLTYPE( z );
      PRINT_DECLTYPE( (z) );
      PRINT_DECLTYPE( foo );
      // PRINT_DECLTYPE( (foo) );
    }
    
  };
  X().foo();
}
