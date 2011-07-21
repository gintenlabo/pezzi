#ifndef PEZZI_INCLUDED_GET_TYPENAME_HPP_
#define PEZZI_INCLUDED_GET_TYPENAME_HPP_

// get_typename : 与えられた型のデマングルされた名前を得る． gcc only．

#include "demangle.hpp"

#include <string>
#include <boost/xpressive/xpressive.hpp>

namespace pezzi
{
  template<class T>
  struct quote_type_ {};
  
  inline std::string dequote_( char const* str )
  {
    namespace xpr = boost::xpressive;
    
    static xpr::cregex const rex =
      "pezzi::quote_type_" >> *xpr::_s >> '<'
        >> *xpr::_s >> ( xpr::s1 = -+xpr::_ ) >> *xpr::_s >> '>';
    
    xpr::cmatch m;
    if( xpr::regex_match( str, m, rex ) ) {
      return m[1].str();
    }
    
    return str;
    
  }
  
  template< class T >
  std::string get_typename()
  {
    return dequote_( pezzi::Demangle( typeid( quote_type_<T> ) ).name() );
  }

} // namespace pezzi

#endif  // #ifndef PEZZI_INCLUDED_GET_TYPENAME_HPP_
