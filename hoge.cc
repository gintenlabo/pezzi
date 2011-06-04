// 適当に検証用に使う使い捨てコード
// C++ インタプリタ欲しいなー。無理だろうけど。
// このコメントはエンコードを UTF-8 にするために書いてるのです。

#include <boost/lexical_cast.hpp>
#include <iostream>

int main()
{
  try {
    (void)boost::lexical_cast<std::size_t>( "-1" );
    std::cout << "no error\n";
  }
  catch ( boost::bad_lexical_cast& ) {
    std::cout << "bad lexical cast\n";
  }
}
