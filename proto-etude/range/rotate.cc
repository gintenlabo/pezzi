// Iterators Must Go ! のアレ．

#include <iterator>
#include <cassert>
#include <boost/range.hpp>

namespace pezzi
{
  template< class Iter >
  class iterator_range_view
  {
    typedef iterator_range_view self_type;
    
   public:
    iterator_range_view()
      : first_(), last_() {}
    
    iterator_range_view( Iter first, Iter last )
      : first_( std::move(first) ), last_( std::move(last) ) {}
    
    
    // Input View Requirements
    typedef typename std::iterator_traits<Iter>::value_type value_type;
    typedef typename std::iterator_traits<Iter>::reference   reference;
    
    bool empty() const {
      return first_ == last_;
    }
    reference front() const {
      assert( !this->empty() );
      return *first_;
    }
    void pop_front() {
      assert( !this->empty() );
      ++first_;
    }
    
    // Forward View Requirements
    friend bool operator==( self_type const& x, self_type const& y ) {
      return ( x.first_ == y.first_ ) && ( x.last_ == y.last_ );
    }
    friend bool operator!=( self_type const& x, self_type const& y ) {
      return !( x == y );
    }
    
    // Forward View Requirements (Optional)
    friend self_type forward_difference( self_type const& x, self_type const& y ) {
      return self_type( x.first_, y.first_ );
    }
    
   private:
    Iter first_, last_;
    
  };
  
  template< class Iter >
  inline iterator_range_view<Iter> make_view( Iter first, Iter last ) {
    return iterator_range_view<Iter>( std::move(first), std::move(last) );
  }
  
  template< class Range,
    class Iter = typename boost::range_iterator<Range>::type
  >
  inline iterator_range_view<Iter> make_view( Range& r ) {
    return pezzi::make_view( boost::begin(r), boost::end(r) );
  }
  template< class Iter >
  inline iterator_range_view<Iter> make_view( iterator_range_view<Iter> view ) {
    return std::move(view);
  }
  
  // for each
  template< class View, class F >
  inline F for_each_( View view, F f )
  {
    while( !view.empty() ) {
      f( view.front() );
      view.pop_front();
    }
    return std::move(f);
  }
  template< class Range, class F >
  inline F for_each( Range && r, F f ) {
    return for_each_( make_view( std::forward<Range>(r) ), std::move(f) );
  }
  
  // 回転
  template< class View1, class View2 >
  inline void rotate_( View1 v1, View2 v2 )
  {
    using std::swap;
    
    if( v2.empty() ) {
      return;
    }
    View2 v2_ = v2;
    
    for(;;) {
      swap( v1.front(), v2.front() );
      v1.pop_front(); v2.pop_front();
      
      if( v1.empty() ) {
        if( v2.empty() ) {
          return;
        }
        else {
          break;
        }
      }
      else if( v2.empty() ) {
        v2 = v2_;
      }
    }
    
    View2 v3 = v2;
    for(;;) {
      swap( v2_.front(), v2.front() );
      v2_.pop_front(); v2.pop_front();
      
      if( v2.empty() ){
        if( v2_ == v3 ) {
          return;
        }
        else {
          v2 = v3;
        }
      }
      else if( v2_ == v3 ) {
        v3 = v2;
      }
    }
  }
  template< class Range1, class Range2 >
  inline void rotate( Range1& r1, Range2& r2 )
  {
    return rotate_( make_view(r1), make_view(r2) );
  }
  
}

#include <iostream>
#include <boost/lambda/lambda.hpp>
namespace bll = boost::lambda;

template< class Range >
void print( Range const& x )
{
  pezzi::for_each( x, std::cout << bll::_1 << ' ' );
  std::cout << std::endl;
}

#include <vector>
#include <deque>
#include <list>

int main()
{
  std::vector<int> vec = { 1 };
  std::deque<int>  deq = { 2, 3, 4, 5, 6, 7 };
  
  pezzi::rotate( vec, deq );
  print( vec ); print( deq );
  
  pezzi::rotate( deq, vec );
  print( deq ); print( vec );
  
  
}
