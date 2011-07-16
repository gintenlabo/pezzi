#ifndef PEZZI_INCLUDED_SHAKER_SORT_HPP_
#define PEZZI_INCLUDED_SHAKER_SORT_HPP_

#include "less.hpp"
#include <algorithm>

namespace pezzi
{
  // シェイカーソート
  // これも特にひねりなし．
  template< class Iter, class Comp >
  void shaker_sort( Iter first, Iter last, Comp comp )
  {
    // 空ならば何もしない
    if( first == last ){ return; }
    // このアルゴリズムの実装内部では， last は最後の要素の次ではなく，最後の要素を示す
    --last;
    
    for(;;) {
      // 順方向
      {
        Iter last_swap = first;
        
        for( Iter i = first, j = std::next(i); j != last; ++i, ++j ) {
          if( comp( *j, *i ) ) {
            std::iter_swap( i, j );
            last_swap = i;
          }
        }
        
        if( first == last_swap ){ return; }
        last = last_swap;
      }
      
      // 逆方向
      {
        Iter last_swap = last;
        
        for( Iter i = last, j = std::prev(i); j != first; --i, --j ) {
          if( comp( *i, *j ) ) {
            std::iter_swap( i, j );
            last_swap = i;
          }
        }
        
        if( last == last_swap ){ return; }
        first = last_swap;
      }
      
    }
  }
  
  template< class Iter >
  void shaker_sort( Iter first, Iter last )
  {
    return pezzi::shaker_sort( first, last, less<>() );
  }

} // namespace pezzi

#endif  // #ifndef PEZZI_INCLUDED_BUBBLE_SORT_HPP_
