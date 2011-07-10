template< class T >
struct X
{
  template< class U >
  struct rebind {
    typedef X<U> type;
  };
  
};

template< template<class...> class Template, class... Types >
struct apply
{
  typedef typename Template<Types...>::type type;
};

template<class T>
struct hoge
{
  typedef typename apply< X<T>::template rebind, T >::type type;
};


#include <type_traits>
#define STATIC_ASSERT(...) static_assert( __VA_ARGS__, #__VA_ARGS__ )

int main()
{
  STATIC_ASSERT( std::is_same< hoge<int>::type, X<int> >::value );
}
