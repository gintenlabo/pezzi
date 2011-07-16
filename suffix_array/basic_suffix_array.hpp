#ifndef PEZZI_INCLUDED_BASIC_SUFFIX_ARRAY_HPP_
#define PEZZI_INCLUDED_BASIC_SUFFIX_ARRAY_HPP_

#include <string>
#include <algorithm>
#include <memory>
#include <utility>

namespace pezzi
{
  template< class charT, class traits = std::char_traits<charT> >
  struct basic_suffix_array
  {
    // 構築
    explicit basic_suffix_array( charT const* s )
      : s_(s), n_( traits::length(s) ), suffix_( make_suffix_array_( s_, n_ ) ) {}
    
    basic_suffix_array( charT const* s, std::size_t n )
      : s_(s), n_(n), suffix_( make_suffix_array_( s_, n_ ) ) {}
    
    // 文字列を得る
    charT const* c_str() const {
      return s_;
    }
    
    // イテレータ
    typedef std::size_t     value_type;
    typedef value_type const* iterator;
    typedef iterator    const_iterator;
    
    iterator  begin() const { return suffix_.get(); }
    iterator    end() const { return suffix_.get() + n_; }
    iterator cbegin() const { return begin(); }
    iterator   cend() const { return end(); }
    
    // 探索
    std::pair<iterator, iterator> search( charT const* t, std::size_t n ) const
    {
      std::size_t const npos = -1;
      // とりあえず std::equal_range を使う
      return std::equal_range( this->begin(), this->end(), npos,
        [=]( std::size_t i, std::size_t j ) -> bool {
          char const* const p = ( i == npos ) ? t : s_ + i;
          char const* const q = ( j == npos ) ? t : s_ + j;
          
          return traits::compare( p, q, n ) < 0;
          
        }
      );
    }
    std::pair<iterator, iterator> search( charT const* t ) const
    {
      return this->search( t, traits::length(t) );
    }
    
   private:
    charT const* s_; std::size_t  n_;
    std::unique_ptr<value_type[]> suffix_;
    
    // suffix array を作る
    static std::unique_ptr<value_type[]>
      make_suffix_array_( charT const* s, std::size_t n )
    {
      std::unique_ptr<value_type[]> buf( new value_type[n] );
      
      for( std::size_t i = 0; i < n; ++i ) {
        buf[i] = i;
      }
      
      // とりあえず std::sort を使う
      std::sort( buf.get(), buf.get() + n,
        [=]( std::size_t i, std::size_t j ){
          return traits::compare( s + i, s + j, n ) < 0;
        }
      );
      
      return buf;
    }
    
  };
  
  typedef basic_suffix_array<char> c_suffix_array;

} // namespace pezzi

#endif  // #ifndef PEZZI_INCLUDED_BASIC_SUFFIX_ARRAY_HPP_
