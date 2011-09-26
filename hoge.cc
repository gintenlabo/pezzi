// 適当に検証用に使う使い捨てコード
// C++ インタプリタ欲しいなー。無理だろうけど。
// このコメントはエンコードを UTF-8 にするために書いてるのです。

#include <iostream>

// 滅茶苦茶大きなファイルを作る
// 256 文字 * 1024 * 1024 行

int main()
{
  std::ios::sync_with_stdio(false);
  
  std::string s;
  for( int i = 0; i < 255; ++i ) {
    s += char( 'a' + i % 26 );
  }
  
  for( int i = 0; i < 1024*1024; ++i ) {
    std::cout << s << std::endl;
  }
}
