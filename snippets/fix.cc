// 不動点コンビネータ in C++11
// http://d.hatena.ne.jp/iorate/20110729/1311949434 に対し，
// std::ref を使って高速化．

#include <utility>
#include <functional>

namespace pezzi
{
  struct fix_t
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
  constexpr fix_t fix = {};
  
} // namespace pezzi

#include <boost/progress.hpp>
#include <iostream>

int main()
{
  boost::progress_timer t;
  
  // 竹内関数でテスト
  auto tarai = pezzi::fix(
    []( std::function<int (int, int, int)> f, int x, int y, int z ) { 
      return ( x <= y ) ? y : f( f( x-1, y, z ), f( y-1, z, x ), f( z-1, x, y ) );
    }
  );
  
  std::cout << tarai( 12, 6, 0 ) << '\n';
  
}
