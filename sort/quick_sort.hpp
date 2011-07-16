#ifndef PEZZI_INCLUDED_QUICK_SORT_HPP_
#define PEZZI_INCLUDED_QUICK_SORT_HPP_

#include <algorithm>
#include "less.hpp"

namespace pezzi
{
  // 三つのイテレータを昇順に入れ替える
  template< class IterA, class IterB, class IterC, class Comp >
  inline void sort_3_elems( IterA a, IterB b, IterC c, Comp comp )
  {
    if( comp( *b, *a ) ) {
      std::iter_swap( b, a );
    }
    
    if( comp( *c, *b ) ) {
      std::iter_swap( c, b );
      if( comp( *b, *a ) ) {
        std::iter_swap( b, a );
      }
    }
    
  }
  
  // 要素数三以下の場合は挿入ソート
  template< class Iter, class Comp >
  inline void mini_insertion_sort_( Iter first, Iter last, std::size_t n, Comp comp )
  {
    assert( n <= 3 );
    
    if( n < 2 ) {
      return;
    }
    else if( n == 2 ) {
      --last;
      if( comp( *last, *first ) ) {
        std::iter_swap( last, first );
        return;
      }
    }
    else {  // n == 3
      return sort_3_elems( first, std::next(first), std::prev(last), comp );
    }
  }
  
  // 普通のクイックソート
  // 特にひねりなし．
  template< class Iter, class Comp >
  void quick_sort( Iter first, Iter last, Comp comp )
  {
    // 要素数
    std::size_t const n = last - first;
    
    // 要素数が少ない場合は挿入ソート（と言いつつ実はバブルソート）を行う
    if( n <= 3 ) {
      // 本当は 3 個と言わず，もっと大きい n で挿入ソートに切り替えたほうが（たぶん）速い
      return mini_insertion_sort_( first, last, n, comp );
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
    quick_sort( first, p_, comp );
    quick_sort( p, last, comp );
  }
  
  template< class Iter >
  void quick_sort( Iter first, Iter last )
  {
    return pezzi::quick_sort( first, last, less<>() );
  }

} // namespace pezzi

#endif  // #ifndef PEZZI_INCLUDED_QUICK_SORT_HPP_
