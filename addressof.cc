#include <type_traits>

namespace pezzi
{
  template< bool Cond, class T >
  struct add_const_if_c {
    typedef T type;
  };
  template< class T >
  struct add_const_if_c<true, T> {
    typedef T const type;
  };
  
  template< bool Cond, class T >
  struct add_volatile_if_c {
    typedef T type;
  };
  template< class T >
  struct add_volatile_if_c<true, T> {
    typedef T volatile type;
  };
  
  template< class From, class To >
  struct add_same_cv_qualifiers_as
  {
    typedef typename add_const_if_c<
      std::is_const<From>::value,
      typename add_volatile_if_c<
        std::is_volatile<From>::value, To
      >::type
    >::type type;
  };
  
  
  template< class T >
  inline T* addressof( T& x )
  {
    return reinterpret_cast<T*>(
      &reinterpret_cast<typename add_same_cv_qualifiers_as<T, char>::type&>(x)
    );
    
  }
  
}

struct X
{
  void operator&() {}
  
};

struct Y
{
  int i; char c;
  
  operator char&() {
    return c;
  }
  
  operator const char&() const {
    return c;
  }
  
  operator volatile char&() volatile {
    return c;
  }
  
  operator const volatile char&() const volatile {
    return c;
  }
  
};

#include <iostream>
#include <type_traits>

#define STATIC_ASSERT(...) static_assert( __VA_ARGS__, #__VA_ARGS__ )

int main()
{
  X x;
  std::cout << pezzi::addressof(x) << std::endl;
  
  Y y;
  std::cout << &y << std::endl;
  std::cout << static_cast<void*>(&y.c) << std::endl;
  std::cout << pezzi::addressof(y) << std::endl;
  
  Y const& cy = y;
  auto p1 = pezzi::addressof(cy);
  STATIC_ASSERT( std::is_same<decltype(p1), Y const*>::value );
  
  Y volatile& vy = y;
  auto p2 = pezzi::addressof(vy);
  STATIC_ASSERT( std::is_same<decltype(p2), Y volatile*>::value );
  
  Y const volatile& cvy = y;
  auto p3 = pezzi::addressof(cvy);
  STATIC_ASSERT( std::is_same<decltype(p3), Y const volatile*>::value );
  
  
}
