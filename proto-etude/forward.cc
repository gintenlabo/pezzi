// std::forward が constexpr に対応してないのでカッとなって作った．

#include <type_traits>
#include "noexcept.hpp"

#include <boost/config.hpp>

namespace proto_etude
{
  // 本体
  template< class T >
  inline BOOST_CONSTEXPR
  T&& forward( typename std::remove_reference<T>::type& x ) ETUDE_NOTHROW {
    return static_cast<T&&>(x);
  }
  
  // std::forward と違い， rvalue は束縛できないようにする
  // int x; std::forward<double>(x); って， move すべき？ すべきじゃない？
  // という問題があるため．
  template< class T >
  inline T&& forward( typename std::remove_reference<T>::type&& x ) = delete;
  
  #define ETUDE_FORWARD(x) ::proto_etude::forward<decltype(x)>(x)

} // namespace proto_etude

#include <utility>

namespace proto_etude
{
  // copy
  
  // metafunctions
  template< class T >
  struct is_decay_copyable
    : std::is_constructible<typename std::decay<T>::type, T>::type {};
  template< class T >
  struct is_nothrow_decay_copyable
    : std::is_nothrow_constructible<typename std::decay<T>::type, T>::type {};
  
  // SFINAE なし版
  template< class T >
  inline BOOST_CONSTEXPR auto copy( T && x )
    ETUDE_NOTHROW_IF( is_nothrow_decay_copyable<T>::value )
      -> typename std::decay<T>::type
  {
    // return std::forward<T>( x ); // constexpr に出来ない
    return proto_etude::forward<T>( x );
    // return ETUDE_FORWARD(x);  // 簡略記法
  }
  
  // SFINAE あり版
  template< class T, class = void >
  struct decay_if_possible_ {};
  
  template< class T >
  struct decay_if_possible_< T,
    typename std::enable_if<
      is_decay_copyable<T>::value
    >::type
  >
  {
    typedef typename std::decay<T>::type type;
  };
  
  template< class T >
  struct decay_if_possible
    : decay_if_possible_<T> {};
  
  
  template< class T >
  inline BOOST_CONSTEXPR auto copy_if_possible( T && x )
    ETUDE_NOTHROW_IF( is_nothrow_decay_copyable<T>::value )
      -> typename decay_if_possible<T>::type
  {
    return ETUDE_FORWARD(x);  // 簡略記法
  }
  
}

#include <iostream>

int main()
{
  BOOST_CONSTEXPR_OR_CONST int i = 0;
  BOOST_CONSTEXPR_OR_CONST int j = proto_etude::copy(i);
  std::cout << j << std::endl;
  
  std::string s = "hogehogefugafuga";
  std::cout << proto_etude::copy_if_possible( std::move(s) ) << std::endl;
  std::cout << s << std::endl;
}
