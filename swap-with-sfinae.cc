#include <utility>
#include <type_traits>

namespace pezzi
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
  
  namespace swap_with_sfinae_impl_
  {
    using std::swap;
    
    template< class T,
      class = typename std::enable_if<!(
        pezzi::is_move_constructible<T>::value &&
        pezzi::is_move_assignable<T>::value
      )>::type
    >
    void swap( T& one, T& another ) = delete;
    
    template< class T, class U,
      bool Noexcept = noexcept(
        swap( std::declval<T>(), std::declval<U>() )
      )
    >
    inline void swap_with_sfinae( T && x, U && y ) noexcept( Noexcept )
    {
      swap( std::forward<T>(x), std::forward<U>(y) );
    }
  }
  
  using swap_with_sfinae_impl_::swap_with_sfinae;

} // namespace pezzi

struct NonCopyable
{
  NonCopyable () {}
  
  NonCopyable( NonCopyable && ) = delete;
  void operator=( NonCopyable && ) = delete;
  
};

int main()
{
  NonCopyable x, y;
  pezzi::swap_with_sfinae( x, y );
}
