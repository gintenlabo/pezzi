// 適当に検証用に使う使い捨てコード
// C++ インタプリタ欲しいなー。無理だろうけど。
// このコメントはエンコードを UTF-8 にするために書いてるのです。

#include <cstdint>
#include <boost/config.hpp>

inline BOOST_CONSTEXPR std::uint32_t rotate_left_32( std::uint32_t x, std::size_t n ) {
  return ( x << ( n % 32 ) ) | ( x >> ( 32 - ( n % 32 ) ) );
}

#include <iostream>

int main()
{
  std::cout << std::hex;
  std::cout << rotate_left_32( 0x11,  4 ) << std::endl;
  std::cout << rotate_left_32( 0x11, 36 ) << std::endl;
  std::cout << rotate_left_32( 0x11, -4 ) << std::endl;
  std::cout << rotate_left_32( 0x11, 28 ) << std::endl;
  std::cout << rotate_left_32( 0x11,  0 ) << std::endl;
  std::cout << rotate_left_32( 0x11, 32 ) << std::endl;
}
