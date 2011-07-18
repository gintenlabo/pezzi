#ifndef PEZZI_INCLUDED_BASIC_SUFFIX_ARRAY_HPP_
#define PEZZI_INCLUDED_BASIC_SUFFIX_ARRAY_HPP_

#include <string>
#include <algorithm>
#include <memory>
#include <utility>
#include <cassert>

namespace pezzi
{
  template< class charT, class traits = std::char_traits<charT> >
  struct basic_suffix_array
  {
    // 構築
    explicit basic_suffix_array( charT const* s )
      : s_( s ), len_( traits::length(s) ), max_query_len_( len_ ),
        suffix_( make_suffix_array_( s_, len_, max_query_len_ ) ) {}
    
    basic_suffix_array( charT const* s, std::size_t len )
      : s_( s ), len_( len ), max_query_len_( len_ ),
        suffix_( make_suffix_array_( s_, len_, max_query_len_ ) ) {}
    
    // search で探す検索クエリの最大長 max_query_len が分かってる場合は，
    // こちらによって構築することで， len が大きい時に，速度改善することができる
    // search に max_query_len より長い文字列が渡された場合，
    // undefined behavior になるので注意． （一応 assert するが）
    basic_suffix_array( charT const* s, std::size_t len, std::size_t max_query_len )
      : s_( s ), len_( len ), max_query_len_( max_query_len ),
        suffix_( make_suffix_array_( s_, len_, max_query_len_ ) ) {}
    
    
    // 各種情報取得
    
    // 検索対象文字列を得る
    charT const* target() const {
      return s_;
    }
    
    // 検索対象文字列の長さを得る
    std::size_t size() const {
      return len_;
    }
    
    // 検索クエリの最大長を得る
    std::size_t max_query_length() const {
      return max_query_len_;
    }
    
    // イテレータ
    // this->target() に対する，ソート済みの suffix array へのアクセスを得る
    typedef std::size_t     value_type;
    typedef value_type const* iterator;
    typedef iterator    const_iterator;
    
    iterator  begin() const { return suffix_.get(); }
    iterator    end() const { return suffix_.get() + len_; }
    iterator cbegin() const { return begin(); }
    iterator   cend() const { return end(); }
    
    
    // suffix array の中から，冒頭が [ q, q+n ) と一致する要素の range を検索する
    // Ο( n * log( this->size() ) )
    std::pair<iterator, iterator> search( charT const* q, std::size_t n ) const
    {
      assert( n <= max_query_len_ );
      return equal_range_( this->s_, this->begin(), this->end(), q, n );
    }
    std::pair<iterator, iterator> search( charT const* q ) const
    {
      return this->search( q, traits::length(q) );
    }
    
    
   private:
    charT const* s_; std::size_t  len_;
    std::size_t max_query_len_;
    std::unique_ptr<value_type[]> suffix_;
    
    
    // 実装用関数群
    
    // suffix array を作る
    static std::unique_ptr<value_type[]>
      make_suffix_array_( charT const* s, std::size_t len, std::size_t max_query_len )
    {
      std::unique_ptr<value_type[]> buf( new value_type[len] );
      
      // とりあえず順に埋めてから
      for( std::size_t i = 0; i < len; ++i ) {
        buf[i] = i;
      }
      
      // 先頭 max_query_len の辞書順にソートする
      std::sort( buf.get(), buf.get() + len,
        [=]( std::size_t i, std::size_t j ){
          return traits::compare( s + i, s + j, max_query_len ) < 0;
        }
      );
      
      return buf;
    }
    
    // s に対応するソート済みの suffix array [ first, last ) の中から，
    // 冒頭 n 文字が q とマッチする最初の要素を指すイテレータを返す
    static iterator
      lower_bound_(
        charT const* s, iterator first, iterator last, charT const* q, std::size_t n
      )
    {
      for(;;) {
        std::size_t const len = last - first;
        if( len == 0 ){ return first; }
        
        std::size_t const half   = len / 2;
        iterator    const middle = first + half;
        
        if( traits::compare( s + *middle, q, n ) < 0 ) {
          first = middle + 1;
        }
        else {
          last = middle;
        }
      }
    }
    
    // s に対応するソート済みの suffix array [ first, last ) の中から，
    // 冒頭 n 文字が q とマッチする最後の要素の次を指すイテレータを返す
    static iterator
      upper_bound_(
        charT const* s, iterator first, iterator last, charT const* q, std::size_t n
      )
    {
      for(;;) {
        std::size_t const len = last - first;
        if( len == 0 ){ return first; }
        
        std::size_t const half   = len / 2;
        iterator    const middle = first + half;
        
        if( traits::compare( s + *middle, q, n ) <= 0 ) {
          first = middle + 1;
        }
        else {
          last = middle;
        }
      }
    }
    
    // s に対応するソート済みの suffix array [ first, last ) の中から，
    // 冒頭 n 文字が q とマッチする最初の要素を指すイテレータ lower と，
    // 冒頭 n 文字が q とマッチする最後の要素の次を指すイテレータ upper の組を返す
    static std::pair<iterator, iterator>
      equal_range_(
        charT const* s, iterator first, iterator last, charT const* q, std::size_t n
      )
    {
      // [ first, last ) の幅を狭める
      for(;;)
      {
        std::size_t const len = last - first;
        if( len == 0 ){ return std::make_pair( first, last ); }
        
        std::size_t const half   = len / 2;
        iterator    const middle = first + half;
        
        int const compare_result = traits::compare( s + *middle, q, n );
        
        if( compare_result < 0 ) {
          first = middle + 1;
        }
        else if( compare_result > 0 ) {
          last = middle;
        }
        else {
          // 縮め完了
          // lower bound は [ first, middle ) に，
          // upper bound は [ middle + 1, last ) にある
          return std::make_pair(
            lower_bound_( s, first,    middle, q, n ),
            upper_bound_( s, middle + 1, last, q, n )
          );
        }
      }
    }
    
  };
  
  typedef basic_suffix_array<char> c_suffix_array;

} // namespace pezzi

#endif  // #ifndef PEZZI_INCLUDED_BASIC_SUFFIX_ARRAY_HPP_
