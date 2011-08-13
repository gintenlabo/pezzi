// is_swappable : etude::value_swap が呼べるか否か

#include "value_swap.hpp"

namespace proto_etude
{
  // 実装用
  struct is_value_swappable_
  {
    template< class T, class U,
      class = decltype(
        proto_etude::value_swap( std::declval<T>(), std::declval<U>() ), 0
      )
    >
    static std::true_type test_( int );
    
    template< class T, class U, class = void >
    static std::false_type test_( ... );
    
    template< class T, class U >
    struct apply
    {
      typedef decltype( test_<T, U>(0) ) type;
    };
    
  };
  
  template< class T, class U >
  struct is_value_swappable
    : is_value_swappable_::template apply<T, U>::type {};
  
  // 本体
  template< class T >
  struct is_swappable
    : is_value_swappable_::template apply<
        typename std::add_lvalue_reference<T>::type,
        typename std::add_lvalue_reference<T>::type
      >::type {};
  
} // namespace proto_etude


// 検証

struct NonCopyable
{
  NonCopyable() {}
  
  NonCopyable( NonCopyable && ) = delete;
  void operator=( NonCopyable && ) = delete;
  
};

struct NonCopyableButSwappable
{
  NonCopyableButSwappable() {}
  
  NonCopyableButSwappable( NonCopyableButSwappable && ) = delete;
  void operator=( NonCopyableButSwappable && ) = delete;
  
  friend void swap( NonCopyableButSwappable&, NonCopyableButSwappable& ) {}
  
};

#include <iostream>

int main()
{
  std::cout << std::boolalpha;
  
  std::cout << proto_etude::is_swappable<int>::value << std::endl;
  std::cout << proto_etude::is_swappable<NonCopyable>::value << std::endl;
  std::cout << proto_etude::is_swappable<NonCopyableButSwappable>::value << std::endl;
  
}
