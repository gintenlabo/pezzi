#ifndef PEZZI_INCLUDED_COMB_SORT_HPP_
#define PEZZI_INCLUDED_COMB_SORT_HPP_

#include <algorithm>
#include "less.hpp"
#include "insertion_sort.hpp"
#include "bubble_sort.hpp"
#include "shaker_sort.hpp"

namespace pezzi
{
  // 最後のソートに insert を使うバリエーション
  // これが総合的に一番優秀のよう．
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
    
    // insertion sort
    return pezzi::insertion_sort( first, last, comp );
  }
  
  template< class Iter >
  void comb_sort( Iter first, Iter last )
  {
    return pezzi::comb_sort( first, last, less<>() );
  }
  
  // 単純なコムソート
  // comb sort 11 は，ベンチマークしたところ，速度改善が見られなかった．
  template< class Iter, class Comp >
  void comb_sort_orig( Iter first, Iter last, Comp comp )
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
    
    for(;;) {
      bool update = false;
      
      for( Iter i = first, j = std::next(i); j != last; ++i, ++j ) {
        if( comp( *j, *i ) ){
          std::iter_swap( i, j );
          update = true;
        }
      }
      if( !update ){ return; }
    }
    
  }
  
  template< class Iter >
  void comb_sort_orig( Iter first, Iter last )
  {
    return pezzi::comb_sort_orig( first, last, less<>() );
  }
  
  // bubble_sort を呼ぶタイプ
  template< class Iter, class Comp >
  void comb_sort_b( Iter first, Iter last, Comp comp )
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
    
    return pezzi::bubble_sort( first, last, comp );
    
  }
  
  template< class Iter >
  void comb_sort_b( Iter first, Iter last )
  {
    return pezzi::comb_sort_b( first, last, less<>() );
  }
  
  // 最後のソートに shaker を使うバリエーション
  template< class Iter, class Comp >
  void comb_sort_shaker( Iter first, Iter last, Comp comp )
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
    
    // shaker sort
    return pezzi::shaker_sort( first, last, comp );
  }
  
  template< class Iter >
  void comb_sort_shaker( Iter first, Iter last )
  {
    return pezzi::comb_sort_shaker( first, last, less<>() );
  }
  
  
} // namespace pezzi

#endif  // #ifndef PEZZI_INCLUDED_COMB_SORT_HPP_
