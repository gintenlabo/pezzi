#ifndef PEZZI_INCLUDED_INSERTION_SORT_HPP_
#define PEZZI_INCLUDED_INSERTION_SORT_HPP_

#include <algorithm>
#include "less.hpp"

namespace pezzi
{
  // 挿入ソート
  template< class Iter, class Comp >
  void insertion_sort( Iter first, Iter last, Comp comp )
  {
    if( first == last ){ return; }
    
    for( Iter i = first, j = std::next(i); j != last; ++i, ++j ) {
      if( comp( *j, *i ) ) {
        std::iter_swap( i, j );
        if( i != first ) {
          if( comp( *i, *first ) ) {
            Iter k = i, l = std::prev(i);
            for(;;) {
              std::iter_swap( k, l );
              if( l == first ){ break; }
              --k; --l;
            }
          }
          else {
            for( Iter k = i, l = std::prev(i); comp( *k, *l ); --k, --l ) {
              std::iter_swap( k, l );
            }
          }
        }
      }
    }
  }
  
  template< class Iter >
  void insertion_sort( Iter first, Iter last )
  {
    return pezzi::insertion_sort( first, last, less<>() );
  }

} // namespace pezzi

#endif  // #ifndef PEZZI_INCLUDED_INSERTION_SORT_HPP_
