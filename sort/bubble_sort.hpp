#ifndef PEZZI_INCLUDED_BUBBLE_SORT_HPP_
#define PEZZI_INCLUDED_BUBBLE_SORT_HPP_

#include <algorithm>
#include "less.hpp"

namespace pezzi
{
  // バブルソート
  // 特にひねりなし．
  template< class Iter, class Comp >
  void bubble_sort( Iter first, Iter last, Comp comp )
  {
    while( first != last ) {
      Iter last_swap = first;
      for( Iter i = first, j = std::next(i); j != last; ++i, ++j ) {
        if( comp( *j, *i ) ){
          std::iter_swap( i, j );
          last_swap = j;
        }
      }
      last = last_swap;
    }
  }
  
  template< class Iter >
  void bubble_sort( Iter first, Iter last )
  {
    return pezzi::bubble_sort( first, last, less<>() );
  }

} // namespace pezzi

#endif  // #ifndef PEZZI_INCLUDED_BUBBLE_SORT_HPP_
