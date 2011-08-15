#ifndef PROTO_ETUDE_INCLUDED_VALUE_SWAP_HPP_
#define PROTO_ETUDE_INCLUDED_VALUE_SWAP_HPP_

// etude::value_swap
//  要するに using namespace std; swap( x, y ); と一緒
//  SFINAE や NOEXCEPT に正しく対応している

#include <utility>
#include <type_traits>

#include "noexcept.hpp"

namespace proto_etude
{
  // std::is_move_constructible/is_move_assignable が無いので
  template< class T >
  struct is_move_constructible
    : std::is_constructible<T, T&&>::type {};
  
  struct is_move_assignable_
  {
    template< class T,
      class = decltype( std::declval<T&>() = std::declval<T&&>() )
    >
    static std::true_type test_( int );
    
    template< class T, class = void >
    static std::false_type test_( ... );
    
    template< class T >
    struct apply
    {
      typedef decltype( test_<T>(0) ) type;
    };
    
  };
  template< class T >
  struct is_move_assignable
    : is_move_assignable_::template apply<T>::type {};
  
  // 実装本体
  namespace value_swap_impl_
  {
    namespace impl_
    {
      template< class T,
        class = typename std::enable_if<!(
          proto_etude::is_move_constructible<T>::value &&
          proto_etude::is_move_assignable<T>::value
        )>::type
      >
      void swap( T& one, T& another ) = delete;
    }
    using   std::swap;
    using impl_::swap;
    
    template< class T, class U,
      bool Noexcept = ETUDE_NOEXCEPT(
        swap( std::declval<T>(), std::declval<U>() )
      )
    >
    inline void value_swap( T && x, U && y )
      ETUDE_NOTHROW_IF( Noexcept )
    {
      swap( std::forward<T>(x), std::forward<U>(y) );
    }
  }
  
  using value_swap_impl_::value_swap;


} // namespace proto_etude

#endif  // #ifndef PROTO_ETUDE_INCLUDED_VALUE_SWAP_HPP_
