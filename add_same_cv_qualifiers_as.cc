#include <type_traits>

namespace pezzi
{
  template< bool Cond, class T >
  struct add_const_if_c {
    typedef T type;
  };
  template< class T >
  struct add_const_if_c<true, T> {
    typedef typename std::add_const<T>::type type;
  };
  
  template< bool Cond, class T >
  struct add_volatile_if_c {
    typedef T type;
  };
  template< class T >
  struct add_volatile_if_c<true, T> {
    typedef typename std::add_volatile<T>::type type;
  };
  
  // From と同じ cv 修飾子を To に与える
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
  
}

#include <type_traits>
#define STATIC_ASSERT(...) static_assert( __VA_ARGS__, #__VA_ARGS__ )

template< class From, class To >
void check()
{
  typedef typename std::add_const<From>::type       const_From;
  typedef typename std::add_volatile<From>::type volatile_From;
  typedef typename std::add_cv<From>::type             cv_From;
  
  STATIC_ASSERT( !std::is_const<From>::value );
  STATIC_ASSERT(  std::is_const<const_From>::value );
  STATIC_ASSERT( !std::is_volatile<From>::value );
  STATIC_ASSERT(  std::is_volatile<volatile_From>::value );
  
  typedef typename std::add_const<To>::type       const_To;
  typedef typename std::add_volatile<To>::type volatile_To;
  typedef typename std::add_cv<To>::type             cv_To;
  
  STATIC_ASSERT(
    std::is_same<
      typename pezzi::add_same_cv_qualifiers_as<To, From>::type, To
    >::value
  );
  STATIC_ASSERT(
    std::is_same<
      typename pezzi::add_same_cv_qualifiers_as<To, const_From>::type,
      const_To
    >::value
  );
  STATIC_ASSERT(
    std::is_same<
      typename pezzi::add_same_cv_qualifiers_as<To, volatile_From>::type,
      volatile_To
    >::value
  );
  STATIC_ASSERT(
    std::is_same<
      typename pezzi::add_same_cv_qualifiers_as<To, cv_From>::type,
      cv_To
    >::value
  );
}

int main()
{
  check<void, void>();
  check<int, int>();
  check<int, int const>();
  check<int, int volatile>();
  check<int, int const volatile>();
}
