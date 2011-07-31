// 適当に検証用に使う使い捨てコード
// C++ インタプリタ欲しいなー。無理だろうけど。
// このコメントはエンコードを UTF-8 にするために書いてるのです。

#include <iostream>
#include "get_typename.hpp"

int main()
{
  struct X {
    int i;
  };
  
  X const x = {0};
  std::cout << pezzi::get_typename<decltype(x.i)>() << std::endl;
}
