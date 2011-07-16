#ifndef PEZZI_INCLUDED_COMB_SORT_HPP_
#define PEZZI_INCLUDED_COMB_SORT_HPP_

#include <algorithm>
#include "less.hpp"
#include "shaker_sort.hpp"

namespace pezzi
{
  // 単純コムソート
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
  
  // コムソートA（普通の comb-11 ）
  template< class Iter, class Comp >
  void comb_sort_a( Iter first, Iter last, Comp comp )
  {
    if( first == last ){ return; }
    std::size_t h = ( last - first ) * 10 / 13;
    if( h < 1 ){ return; }
    
    for(;;)
    {
      if( h <= 10 ) {
        if( h > 8 ) {
          h = 11;
        }
        break;
      }
      for( Iter i = first, j = i + h; j != last; ++i, ++j ) {
        if( comp( *j, *i ) ){
          std::iter_swap( i, j );
        }
      }
      h = h * 10 / 13;
    }
    
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
  void comb_sort_a( Iter first, Iter last )
  {
    return pezzi::comb_sort_a( first, last, less<>() );
  }
  
  // コムソートB （比率 4/3 ）
  template< class Iter, class Comp >
  void comb_sort_b( Iter first, Iter last, Comp comp )
  {
    if( first == last ){ return; }
    std::size_t h = ( last - first ) * 3 / 4;
    if( h < 1 ){ return; }
    
    while( h > 1 ) {
      for( Iter i = first, j = i + h; j != last; ++i, ++j ) {
        if( comp( *j, *i ) ){
          std::iter_swap( i, j );
        }
      }
      h = h * 3 / 4;
    }
    pezzi::shaker_sort( first, last, comp );
  }
  
  template< class Iter >
  void comb_sort_b( Iter first, Iter last )
  {
    return pezzi::comb_sort_b( first, last, less<>() );
  }

} // namespace pezzi

#endif  // #ifndef PEZZI_INCLUDED_COMB_SORT_HPP_
