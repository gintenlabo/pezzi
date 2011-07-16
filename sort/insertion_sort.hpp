#ifndef PEZZI_INCLUDED_INSERTION_SORT_HPP_
#define PEZZI_INCLUDED_INSERTION_SORT_HPP_

#include <algorithm>
#include "less.hpp"

namespace pezzi
{
  // pos を [ first, last ) の中の適切な位置に挿入
  // [ first, last ) はソート済みでなければならない
  // pos の位置には最大の要素が入る
  // なお last == pos でも構わない
  template< class Iter, class Comp >
  void insert_sorted( Iter first, Iter last, Iter pos, Comp comp )
  {
    if( first == last ){ return; }
    --last;
    
    while( comp( *pos, *last ) ) {
      std::iter_swap( last, pos );
      
      if( first == last ) {
        return;
      }
      pos = last; --last;
    }
  }
  
  // 挿入ソート本体
  template< class Iter, class Comp >
  void insertion_sort( Iter first, Iter last, Comp comp )
  {
    if( first == last ){ return; }
    
    for( Iter i = first, j = std::next(i); j != last; ++i, ++j ) {
      if( comp( *j, *i ) ) {
        std::iter_swap( i, j );
        insert_sorted( first, i, i, comp );
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
