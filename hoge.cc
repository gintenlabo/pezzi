// 適当に検証用に使う使い捨てコード
// C++ インタプリタ欲しいなー。無理だろうけど。
// このコメントはエンコードを UTF-8 にするために書いてるのです。

#include <atomic>
#include <type_traits>

#define STATIC_ASSERT(...) static_assert( __VA_ARGS__, #__VA_ARGS__ )

int main()
{
  STATIC_ASSERT( std::is_class<std::atomic_int>::value );
  STATIC_ASSERT( std::is_base_of<std::atomic_int, std::atomic<int>>::value );
}
