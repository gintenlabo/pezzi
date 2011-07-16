#ifndef PEZZI_INCLUDED_COMB_SORT_HPP_
#define PEZZI_INCLUDED_COMB_SORT_HPP_

#include <algorithm>
#include "less.hpp"
#include "shaker_sort.hpp"

namespace pezzi
{
  // 単純なコムソート
  // comb sort 11 は，ベンチマークしたところ，速度改善が見られなかった．
  template< class Iter, class Comp >
  void comb_sort( Iter first, Iter last, Comp comp )
  {
    if( first == last ){ return; }
    std::size_t h = ( last - first ) * 10 / 13;
    if( h < 1 ){ return; }  // size == 1
    
    while( h > 1 ) {
      for( Iter i = first, j = i + h; j != last; ++i, ++j ) {
        if( comp( *j, *i ) ){
          std::iter_swap( i, j );
        }
      }
      h = h * 10 / 13;
    }
    pezzi::shaker_sort( first, last, comp );
  }
  
  template< class Iter >
  void comb_sort( Iter first, Iter last )
  {
    return pezzi::comb_sort( first, last, less<>() );
  }
  
} // namespace pezzi

#endif  // #ifndef PEZZI_INCLUDED_COMB_SORT_HPP_
