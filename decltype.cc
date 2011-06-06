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
#include <vector>

int main()
{
  std::cout << get_typename<int>() << std::endl;
  std::cout << get_typename<int const&>() << std::endl;
  std::cout << get_typename<std::string>() << std::endl;
  std::cout << get_typename<std::vector<int>>() << std::endl;
  std::cout << get_typename<type<std::vector<int>>>() << std::endl;
}
