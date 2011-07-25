// 左ローテートするコード
// for std::uint8_t, std::uint16_t, std::uint32_t, std::uint64_t

#include <cstdint>
#include <limits>
#include <type_traits>
#include <boost/config.hpp>

namespace pezzi
{
  // 実装はテンプレートで
  // 一応， SFINAE する
  // 本当は static_assert したかったが， FDIS より前だと
  // constexpr 関数は return 文しか置けないので…．
  template< std::size_t N, class Int,
    class Limits = std::numeric_limits<Int>,
    class = typename std::enable_if<
      Limits::is_integer && !Limits::is_signed &&
      Limits::digits == N
    >::type
  >
  inline BOOST_CONSTEXPR Int rotate_left_( Int x, std::size_t n ) {
    return ( x << ( n % N ) ) | ( x >> ( N - ( n % N ) ) );
  }
  
  // 本体はプリプロセッサで生成させる
  #define PEZZI_GEN_( N )                   \
    inline BOOST_CONSTEXPR                  \
    std::uint##N##_t rotate_left            \
      ( std::uint##N##_t x, std::size_t n ) \
    {                                       \
      return rotate_left_<N>( x, n );       \
    }                                       \
    /* PEZZI_GEN_( N ) */
  
    // テンプレートではなく， uint8_t, uint16_t, uint32_t 版をそれぞれ定義
    // 明示的にそれぞれの型にキャストして使う
    PEZZI_GEN_(  8 )
    PEZZI_GEN_( 16 )
    PEZZI_GEN_( 32 )
    
    // uint64_t に対しては，サポートがある場合のみ
    #ifndef BOOST_NO_INT64_T
      PEZZI_GEN_( 64 )
    #endif
  
  #undef PEZZI_GEN_
  
}
#include <iostream>

int main()
{
  std::cout << std::hex;
  
  {
    std::uint8_t const x = 0x12;
    std::cout << static_cast<unsigned int>( pezzi::rotate_left( x,  4 ) ) << std::endl;
    std::cout << static_cast<unsigned int>( pezzi::rotate_left( x, 12 ) ) << std::endl;
    std::cout << static_cast<unsigned int>( pezzi::rotate_left( x, -4 ) ) << std::endl;
    std::cout << static_cast<unsigned int>( pezzi::rotate_left( x,  4 ) ) << std::endl;
    std::cout << static_cast<unsigned int>( pezzi::rotate_left( x,  0 ) ) << std::endl;
    std::cout << static_cast<unsigned int>( pezzi::rotate_left( x,  8 ) ) << std::endl;
    std::cout << static_cast<unsigned int>( pezzi::rotate_left( x, 16 ) ) << std::endl;
  }
  
  {
    std::uint16_t const x = 0x1234;
    std::cout << pezzi::rotate_left( x,  4 ) << std::endl;
    std::cout << pezzi::rotate_left( x, 20 ) << std::endl;
    std::cout << pezzi::rotate_left( x, -4 ) << std::endl;
    std::cout << pezzi::rotate_left( x, 12 ) << std::endl;
    std::cout << pezzi::rotate_left( x,  0 ) << std::endl;
    std::cout << pezzi::rotate_left( x, 16 ) << std::endl;
    std::cout << pezzi::rotate_left( x, 32 ) << std::endl;
  }
  
  {
    std::uint32_t const x = 0x12345678;
    std::cout << pezzi::rotate_left( x,  4 ) << std::endl;
    std::cout << pezzi::rotate_left( x, 36 ) << std::endl;
    std::cout << pezzi::rotate_left( x, -4 ) << std::endl;
    std::cout << pezzi::rotate_left( x, 28 ) << std::endl;
    std::cout << pezzi::rotate_left( x,  0 ) << std::endl;
    std::cout << pezzi::rotate_left( x, 32 ) << std::endl;
    std::cout << pezzi::rotate_left( x, 64 ) << std::endl;
  }
  
  {
    std::uint64_t const x = 0x0123456789ABCDEF;
    std::cout << pezzi::rotate_left( x,  4 ) << std::endl;
    std::cout << pezzi::rotate_left( x, 68 ) << std::endl;
    std::cout << pezzi::rotate_left( x, -4 ) << std::endl;
    std::cout << pezzi::rotate_left( x, 60 ) << std::endl;
    std::cout << pezzi::rotate_left( x,  0 ) << std::endl;
    std::cout << pezzi::rotate_left( x, 64 ) << std::endl;
  }
}
