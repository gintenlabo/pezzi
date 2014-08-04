// 
// なにジョジョ？
// operator+= から operator+ を生成すると constexpr にできない？
// 
// ジョジョ　それは
// operator+= を使うからだよ
// 
// 逆に考えるんだ
// 「 operator+ から operator+= を生成すればいいさ」
// と考えるんだ
// 

#include <utility>

namespace pezzi
{
  template< class T >
  struct addable
  {
    friend T& operator+=( T& lhs, T const& rhs ) {
      lhs = std::move(lhs) + rhs;
      return lhs;
    }
    
    friend T& operator+=( T& lhs, T && rhs ) {
      lhs = std::move(lhs) + std::move(rhs);
      return lhs;
    }
    
  };
  
}

namespace ns
{
  struct hoge
    : private pezzi::addable<hoge>
  {
    int i;
    
    explicit constexpr hoge( int i_ )
      : i( i_ ) {}
    
    friend constexpr hoge operator+( hoge const& lhs, hoge const& rhs )
    {
      return hoge( lhs.i + rhs.i );
    }
    
  };
  
}

#include <iostream>

int main()
{
  using ns::hoge;
  
  constexpr hoge h1(1);
  constexpr hoge h2(2);
  constexpr hoge h3 = h2 + h1;
  
  hoge h4(4);
  h4 += h3;
  
  std::cout << h4.i << std::endl;
  
}
