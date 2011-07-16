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
    typedef typename std::iterator_traits<Iter>::value_type value_type;
    if( first == last ){ return; }
    
    for( Iter i = first, j = std::next(i); j != last; ++i, ++j ) {
      if( comp( *j, *i ) ) {
        value_type temp = std::move(*j);
        Iter k = i;
        *j = std::move(*k);
        for(;;)
        {
          if( k == first ){
            *k = std::move(temp);
            break;
          }
          Iter l = k; --k;
          if( !comp( temp, *k ) ) {
            *l = std::move(temp);
            break;
          }
          *l = std::move(*k);
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
