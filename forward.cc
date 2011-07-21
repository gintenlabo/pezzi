// perfect forward を行うマクロ
// ローカル変数についてはよくテストしてないのでアレですが

#include <iostream>
#include "get_typename.hpp"

void f() {
  std::cout << std::endl;
}

template< class T, class... Args >
void f( T && , Args&&... args ) {
  std::cout << pezzi::get_typename<T&&>() << ", ";
  return f( std::forward<Args>(args)... );
}


#include <utility>
#define STD_FORWARD(x) std::forward<decltype(x)>(x)

template< class... Args >
void g1( Args&&... args )
{
  f( STD_FORWARD(args)... );
}

template< class... Args >
void g2( Args const&... args )
{
  f( STD_FORWARD(args)... );
}

template< class... Args >
void g3( Args... args )
{
  f( STD_FORWARD(args)... );
}

void h() {}

int main()
{
  int i;
  int &r = i;
  
  std::cout << "g1(0): ";
  g1( 0 );
  std::cout << "g2(0): ";
  g2( 0 );
  std::cout << "g3(0): ";
  g3( 0 );
  
  std::cout << "g1(i): ";
  g1( i );
  std::cout << "g2(i): ";
  g2( i );
  std::cout << "g3(i): ";
  g3( i );
  
  std::cout << "g1( STD_FORWARD(i) ): ";
  g1( STD_FORWARD(i) );
  std::cout << "g2( STD_FORWARD(i) ): ";
  g2( STD_FORWARD(i) );
  std::cout << "g3( STD_FORWARD(i) ): ";
  g3( STD_FORWARD(i) );
  
  std::cout << "g1( STD_FORWARD(r) ): ";
  g1( STD_FORWARD(r) );
  std::cout << "g2( STD_FORWARD(r) ): ";
  g2( STD_FORWARD(r) );
  std::cout << "g3( STD_FORWARD(r) ): ";
  g3( STD_FORWARD(r) );
  
  std::cout << "g1( h ): ";
  g1( h );
  std::cout << "g2( h ): ";
  g2( h );
  std::cout << "g3( h ): ";
  g3( h );
  
  std::cout << "g1( STD_FORWARD(h) ): ";
  g1( STD_FORWARD(h) );
  // std::cout << "g2( STD_FORWARD(h) ): ";
  // g2( STD_FORWARD(h) );
  std::cout << "g3( STD_FORWARD(h) ): ";
  g3( STD_FORWARD(h) );
  
}
