namespace pezzi
{
  template< class Derived >
  struct equality_comparable
  {
    friend constexpr bool operator!=( Derived const& lhs, Derived const& rhs ) {
      return !( lhs == rhs );
    }
    
  };
  
}

namespace ns
{
  struct literal_class
    : private pezzi::equality_comparable<literal_class>
  {
    int a;
    
    constexpr explicit literal_class( int a_ )
      : a(a_) {}
    
  };
  inline constexpr bool operator==( literal_class const& x, literal_class const& y ) {
    return x.a == y.a;
  }
  
  struct non_literal_class
    : private pezzi::equality_comparable<non_literal_class>
  {
    int a;
    
    explicit non_literal_class( int a_ )
      : a(a_) {}
    
  };
  inline bool operator==( non_literal_class const& x, non_literal_class const& y ) {
    return x.a == y.a;
  }
  
}

#include <iostream>

int main()
{
  constexpr ns::literal_class a(1), b(2);
  static_assert( a != b, "a != b" );
  
  ns::non_literal_class c(1), d(2);
  std::cout << std::boolalpha << ( c != d ) << std::endl;
}
