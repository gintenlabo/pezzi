// decltype で指定される型

#include "demangle.hpp"

template< class T >
struct type {};

#include <string>
#include <boost/xpressive/xpressive.hpp>

inline std::string remove_type_( std::string const& tn )
{
  namespace xpr = boost::xpressive;
  static xpr::sregex const rex = "type<" >> ( xpr::s1 = -+xpr::_ ) >> *xpr::_s >> ">";
  
  xpr::smatch m;
  if( xpr::regex_match( tn, m, rex ) ) {
    return m[1].str();
  }
  
  return tn;
}

template< class T >
std::string get_typename()
{
  return remove_type_( pezzi::Demangle( typeid( type<T> ) ).name() );
}

#include <iostream>

#define PRINT_DECLTYPE( expr )  \
  ( std::cout << #expr << " : " << get_typename<decltype(expr)>() << std::endl )

int main()
{
  int i;
  PRINT_DECLTYPE( i );
  PRINT_DECLTYPE((i));
}
