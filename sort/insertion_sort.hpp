#ifndef PEZZI_INCLUDED_INSERTION_SORT_HPP_
#define PEZZI_INCLUDED_INSERTION_SORT_HPP_

#include <algorithm>
#include "less.hpp"

namespace pezzi
{
  // 挿入ソート
  template< class Iter, class Comp >
  void insertion_sort_a( Iter first, Iter last, Comp comp )
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
  void insertion_sort_a( Iter first, Iter last )
  {
    return pezzi::insertion_sort_a( first, last, less<>() );
  }

  // 挿入ソートの swap を使わない実装
  template< class Iter, class Comp >
  void insertion_sort_b( Iter first, Iter last, Comp comp )
  {
    typedef typename std::iterator_traits<Iter>::value_type value_type;
    if( first == last ){ return; }
    
    for( Iter i = first, j = std::next(i); j != last; ++i, ++j ) {
      if( comp( *j, *i ) ) {
        value_type temp = std::move(*j);
        *j = std::move(*i);
        if( i == first ) {
          *i = std::move(temp);
        }
        else if( comp( temp, *first ) ) {
          Iter k = i, l = std::prev(k);
          for(;;) {
            *k = std::move(*l);
            if( l == first ){ break; }
            --k; --l;
          }
          *l = std::move(temp);
        }
        else {
          Iter k = i, l = std::prev(k);
          for( ; comp( temp, *l ); --k, --l ) {
            *k = std::move(*l);
          }
          *k = std::move(temp);
        }
      }
    }
  }
  
  template< class Iter >
  void insertion_sort_b( Iter first, Iter last )
  {
    return pezzi::insertion_sort_b( first, last, less<>() );
  }


} // namespace pezzi

#endif  // #ifndef PEZZI_INCLUDED_INSERTION_SORT_HPP_
