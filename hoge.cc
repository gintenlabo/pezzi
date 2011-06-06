// 適当に検証用に使う使い捨てコード
// C++ インタプリタ欲しいなー。無理だろうけど。
// このコメントはエンコードを UTF-8 にするために書いてるのです。

#include "demangle.hpp"

#include <iostream>

template< class T >
struct type {};

template< class T >
void print_type()
{
  std::cout << pezzi::Demangle( typeid(type<T>) ) << std::endl;
}

int main()
{
  // 関数型とか
  typedef int function_type();
  print_type<function_type const&>();
}
