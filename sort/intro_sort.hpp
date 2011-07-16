#ifndef PEZZI_INCLUDED_INTRO_SORT_HPP_
#define PEZZI_INCLUDED_INTRO_SORT_HPP_

#include <algorithm>
#include "less.hpp"

#include "insertion_sort.hpp"
#include "comb_sort.hpp"

namespace pezzi
{
  // ログ計算
  inline std::size_t lg_( std::size_t n )
  {
    std::size_t k;
    for( k = 0; n != 0; n /= 2 ) {
      ++k;
    }
    return k-1;
  }
  
  // ヒープソートではなくコムソートに移譲するイントロソート．
  template< class Iter, class Comp >
  void intro_sort_( std::size_t rec, Iter first, Iter last, Comp comp )
  {
    // 要素数
    std::size_t const n = last - first;
    
    // 要素数が少ない場合は挿入ソートを行う
    if( n <= 20 ) {
      return pezzi::insertion_sort( first, last, comp );
    }
    
    // 再帰が深ければコムソートに切り替え
    if( rec <= 0 ) {
      return pezzi::comb_sort( first, last, comp );
    }
    
    // そうでなければクイックソート
    
    // 先頭，中央，末尾から，ピボットを選ぶ
    Iter back = last - 1;
    Iter mid = first + n/2;
    // この三つを，まず入れ替える
    sort_3_elems( first, mid, back, comp );
    
    // この時点で， first <= mid <= back．
    // ピボットとしては mid を使う
    // ピボットを先頭から二番目に持ってくる
    // （先頭はピボットより小さいことが既に分かっている）
    Iter piv = first + 1;
    std::iter_swap( piv, mid );
    Iter p = piv + 1;
    
    for( Iter iter = p; iter != back; ++iter ){
      if( !comp( *piv, *iter ) ) {
        std::iter_swap( p, iter );
        ++p;
      }
    }
    
    
    Iter p_ = p - 1;
    std::iter_swap( piv, p_ );
    
    // 再帰
    intro_sort_( rec-1, first, p_, comp );
    intro_sort_( rec-1, p, last, comp );
  }
  
  // 普通のクイックソート
  // 特にひねりなし．
  template< class Iter, class Comp >
  void intro_sort( Iter first, Iter last, Comp comp )
  {
    return pezzi::intro_sort_( pezzi::lg_(last-first), first, last, comp );
  }
  
  template< class Iter >
  void intro_sort( Iter first, Iter last )
  {
    return pezzi::intro_sort( first, last, less<>() );
  }

} // namespace pezzi

#endif  // #ifndef PEZZI_INCLUDED_INTRO_SORT_HPP_
