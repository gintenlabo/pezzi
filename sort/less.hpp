#ifndef PEZZI_INCLUDED_LESS_HPP_
#define PEZZI_INCLUDED_LESS_HPP_

#include <functional>

namespace pezzi
{
  template< class T = void >
  struct less
    : std::less<T> {};
  
  template<>
  struct less<>
  {
    template< class T >
    bool operator()( T const& x, T const& y ) const
    {
      return std::less<T>()( x, y );
    }
  };

} // namespace pezzi

#endif  // #ifndef PEZZI_INCLUDED_LESS_HPP_
