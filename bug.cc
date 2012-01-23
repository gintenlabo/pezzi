template< class T >
class ClassTemplate {};

template< class T >
struct Metafunction {
  typedef T type;
};

template< class T >
using TemplateAlias = ClassTemplate< typename Metafunction<T>::type >;

using Alias = TemplateAlias<int>; // typedef TemplateAlias<int> Alias だと再現しない

template< class T >
void f( TemplateAlias<T> );

int main()
{
  Alias x;  // TemplateAlias<int> x; だと再現しない
  f( x );   // f<int>(x) だと再現しない
}
