// auto を使った関数宣言が有効な例（ decltype 以外）

// 関数ポインタを返す関数

// こう書かなきゃいけなかったのが
int ( *f1(char) )(double)
{
  return 0;
}
// こんなにシンプルに！
auto g1( char )
  -> int (*)(double)
{
  return 0;
}

#include <cassert>

int main()
{
  if( f1(0) != g1(0) ) {
    assert( !"should not get here." );
  }
}
