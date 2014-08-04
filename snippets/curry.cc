#include <tuple>
#include <utility>

namespace pezzi
{
  struct partial_apply_1_t
  {
    template< class F, class T >
    struct result
    {
      std::tuple<F, T> t;  // EBO のため pair ではなく tuple を使う
      
      template< class... Args,
        class Result = decltype( 
          std::declval<F const&>()( std::declval<T const&>(), std::declval<Args>()... )
        )
      >
      Result operator()( Args&&... args ) const
      {
        return std::get<0>(t)( std::get<1>(t), std::forward<Args>(args)... );
      }
      
    };
    
    template< class F, class T >
    auto operator()( F f, T t ) const
      -> result< F, T >
    {
      return { std::tuple<F, T>( std::move(f), std::move(t) ) };
    }
    
  };
  const partial_apply_1_t partial_apply_1 = {};
  
  struct curry_t
  {
    template< class F >
    struct result
    {
      F f;
      
      template< class T >
      auto operator()( T && x ) const
        -> decltype( pezzi::partial_apply_1( f, std::forward<T>(x) ) )
      {
        return pezzi::partial_apply_1( f, std::forward<T>(x) );
      }
      
    };
    
    template< class F >
    auto operator()( F f ) const
      -> result<F>
    {
      return { std::forward<F>(f) };
    }
    
  };
  curry_t const curry = {};
}

#include <iostream>

int main()
{
  // 二引数 currying
  auto f = pezzi::curry(
    []( int x, int y ) {
      return x * 2 + y;
    }
  );
  
  auto g = f(2);
  std::cout << g(1) << std::endl; // 5
  
  
  // curry によって影響があるのは先頭引数だけ
  auto h = pezzi::curry(
    []( int x, int y, int z ) {
      return x * 2 + y - z;
    }
  );
  
  // h(2)(1)(5) とは書けない（もし h が可変個引数だったらどうする？）
  std::cout << h(2)( 1, 5 ) << std::endl; // 0
  
}
