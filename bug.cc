#include <type_traits>
#include <utility>

struct Less
{
  template< class T, class U >
  auto operator()( T const& x, U const& y ) const
    -> decltype( x < y )
  {
    return x < y;
  }
};

template< class T, class Comp = Less,
  class = typename std::enable_if<
    std::is_constructible< bool,
      decltype(
        std::declval<Comp&>()(
          *std::declval<T const&>(),
          *std::declval<T const&>()
        )
      )
    >::value
  >::type
>
inline bool pointee_before( T const& x, T const& y, Comp && comp = Comp() ) {
  return y ? ( x ? bool( comp( *x, *y ) ) : true ) : false;
}

template< class T, class U, class Comp = Less,
  class = typename std::enable_if<
    std::is_constructible< bool,
      decltype(
        std::declval<Comp&>()(
          *std::declval<T const&>(),
          *std::declval<U const&>()
        )
      )
    >::value
  >::type
>
inline bool pointee_before( T const& x, U const& y, Comp && comp = Comp() ) {
  return y ? ( x ? bool( comp( *x, *y ) ) : true ) : false;
}

int main()
{
  int* p = 0;
  int const* q = 0;
  
  pointee_before( p, q );
}
