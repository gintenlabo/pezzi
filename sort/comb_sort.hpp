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
  
  // コムソートB（魔改造）
  template< class Iter, class Comp >
  void comb_sort_b( Iter first, Iter last, Comp comp )
  {
    if( first == last ){ return; }
    std::size_t h = ( last - first ) * 10 / 13;
    if( h < 1 ){ return; }
    
    // 櫛を入れる関数
    auto comb = [&]() {
      for( Iter i = first, j = i + h; j != last; ++i, ++j ) {
        if( comp( *j, *i ) ){
          std::iter_swap( i, j );
        }
      }
    };
    
    //  櫛の目が 8 より大きい場合
    while( h > 8 )
    {
      if( h <= 10 ){
        h = 11;
        comb();
        h = 8;
        break;
      }
      comb();
      h = h * 10 / 13;
    }
    
    // 8 以下の場合
    if( h == 8 ) {
      comb();
      h = 6;// 8 * 10 / 13;
    }
    if( h == 7 ) {
      comb();
      h = 5;// 7 * 10 / 13;
    }
    if( h == 6 ) {
      comb();
      h = 4; // 6 * 10 / 13;
    }
    if( h > 4 ) {
      comb();
      h = 3; // (5or4) * 10 / 13;
    }
    if( h == 3 ) {
      comb();
      h = 2;// 2 * 10 / 13;
    }
    if( h == 2 ) {
      comb();
      h = 1;
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
