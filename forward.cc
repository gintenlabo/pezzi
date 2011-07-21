// perfect forward を行うマクロ
// ローカル変数についてはよくテストしてないのでアレですが

#include <iostream>

void f( int && ) {
  std::cout << "f( int&& )\n";
}

void f( int& ) {
  std::cout << "f( int& )\n";
}

void f( void (&)() ) {
  std::cout << "f( void(&)() )\n";
}


#include <utility>
#define STD_FORWARD(x) std::forward<decltype(x)>(x)

template< class... Args >
void g( Args&&... args )
{
  f( STD_FORWARD(args)... );
}

void h() {}

int main()
{
  int i;
  int &r = i;
  
  g( 0 );               // calls f(int&&)
  g( i );               // calls f(int&)
  g( STD_FORWARD(i) );  // calls f(int&&)
  g( STD_FORWARD(r) );  // calls f(int&)

  g( h );               // calls f( void(&)() )
  // g( STD_FORWARD(h) );  // 無☆理
}
