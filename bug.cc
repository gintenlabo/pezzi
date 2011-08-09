#define STATIC_ASSERT( expr ) static_assert( expr, #expr )

// tested function
int f( int );

STATIC_ASSERT( sizeof( f({}) ) == sizeof(int) );  // OK

auto g()
  -> decltype( f({}) );  // OK

template<
  class R = decltype( f({}) ) // fails to compile here
>
R h();

int main()
{
}
