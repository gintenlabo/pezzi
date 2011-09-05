#include <utility>
#include <functional>

namespace pezzi
{
  // ref 版
  struct fix1_t
  {
    template< class F >
    struct result
    {
      typedef result<F> self_type;
      
      F f;
      
      template< class... Args,
        class Result = decltype (
          std::declval<F const&>()(
            std::ref( std::declval<self_type const&>() ),
            std::declval<Args>()...
          )
        )
      >
      Result operator()( Args&&... args ) const
      {
        return f( std::ref(*this), std::forward<Args>(args)... );
      }
      
    };
    
    template< class F >
    constexpr auto operator()( F && f )
      -> result<typename std::decay<F>::type>
    {
      return { std::forward<F>(f) };
    }
    
  };
  constexpr fix1_t fix1 = {};
  
  // オリジナル
  struct fix2_t
  {
    template< class F >
    struct result
    {
      typedef result<F> self_type;
      
      F f;
      
      template< class... Args,
        class Result = decltype (
          std::declval<F const&>()(
            std::declval<self_type const&>(),
            std::declval<Args>()...
          )
        )
      >
      Result operator()( Args&&... args ) const
      {
        return f( *this, std::forward<Args>(args)... );
      }
      
    };
    
    template< class F >
    constexpr auto operator()( F && f )
      -> result<typename std::decay<F>::type>
    {
      return { std::forward<F>(f) };
    }
    
  };
  constexpr fix2_t fix2 = {};
  
} // namespace pezzi

#include <boost/progress.hpp>
#include <iostream>

int main()
{
  // boost::progress_timer t;
  
  // 竹内関数でテスト
  auto tarai_ = []( std::function<int (int, int, int)> f, int x, int y, int z ) { 
    return ( x <= y ) ? y : f( f( x-1, y, z ), f( y-1, z, x ), f( z-1, x, y ) );
  };
  
  auto tarai1 = pezzi::fix1( tarai_ );
  auto tarai2 = pezzi::fix2( tarai_ );
  
  {
    std::cout << "ref版：\n";
    boost::progress_timer t;
    std::cout << tarai1( 12, 6, 0 ) << '\n';
  }
  {
    std::cout << "オリジナル：\n";
    boost::progress_timer t;
    std::cout << tarai2( 12, 6, 0 ) << '\n';
  }
  
}
