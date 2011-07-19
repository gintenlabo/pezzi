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
    
    // 空の suffix array を構築する
    // Postcond : this->empty() == true
    basic_suffix_array()
      : s_(0), len_(0), max_query_len_(-1), suffix_() {}
    
    // 与えられた文字列に対する suffix array を構築する
    // Complexity : Ο( N * N * log(N) ), for N = traits::length(s)
    // Precond  : range [ s, s + traits::length(s) ) is valid
    // Postcond : this->target()           == s,
    //            this->size()             == traits::length(s),
    //            this->max_query_length() == max element of std::size_t, // -1
    //            this->valid()            == true
    explicit basic_suffix_array( charT const* s )
      : s_(s), len_( traits::length(s) ), max_query_len_(-1),
        suffix_( make_suffix_array_( s_, len_, max_query_len_ ) ) {}
    
    // 与えられた文字列の先頭 len 文字に対する suffix array を構築する
    // Complexity : Ο( len * len * log(len) )
    // Precond  : range [ s, s + len ) is valid, traits::length(s) <= len
    // Postcond : this->target()           == s,
    //            this->size()             == len,
    //            this->max_query_length() == max element of std::size_t, // -1
    //            this->valid()            == true
    basic_suffix_array( charT const* s, std::size_t len )
      : s_(s), len_(len), max_query_len_(-1),
        suffix_( make_suffix_array_( s_, len_, max_query_len_ ) ) {}
    
    // search で探す検索クエリの最大長が max_query_len であるとき，
    // 与えられた文字列の先頭 len 文字に対する suffix array を構築する
    // len が大きな場合には効率を改善できる可能性がある
    // Complexity : Ο( max_query_len * len * log(len) )
    // Precond  : range [ s, s + len ) is valid, traits::length(s) <= len
    // Postcond : this->target()           == s,
    //            this->size()             == len,
    //            this->max_query_length() == max_query_len,
    //            this->valid()            == true
    basic_suffix_array( charT const* s, std::size_t len, std::size_t max_query_len )
      : s_(s), len_(len), max_query_len_( max_query_len ),
        suffix_( make_suffix_array_( s_, len_, max_query_len_ ) ) {}
    
    // ソートアルゴリズムを外部から指定する版．
    // sort は，確保された未ソートの suffix array の先頭を示す iterator iter に対し，
    // sort( iter, iter + len, s, max_query_len )
    // という形で呼び出される．
    // Complexity : Ο( len + Complexity{ sort( iter, iter+len, s, max_query_len ) } ),
    //              where iter is a value of basic_suffix_array::iterator.
    // Requires : range [ s, s + len ) is valid, traits::length(s) <= len ;
    //            after calling sort( iter, iter+len, s, max_query_len ),
    //              for all i, j ( i <= j ) in [ iter, iter+len )
    //              traits::compare( s + *i, s + *j, max_query_len ) shall be <= 0.
    // Postcond : this->target()           == s,
    //            this->size()             == len,
    //            this->max_query_length() == max_query_len,
    //            this->valid()            == true
    template< class Sort >
    basic_suffix_array
      ( charT const* s, std::size_t len, std::size_t max_query_len, Sort sort )
        : s_(s), len_(len), max_query_len_( max_query_len ),
          suffix_( make_suffix_array_with_sort_( s_, len_, std::move(sort) ) ) {}
    
    
    // 各種情報取得
    
    // 検索対象文字列を得る
    // Precond: None
    charT const* target() const {
      return s_;
    }
    
    // 検索対象文字列の長さを得る
    // Precond: None
    std::size_t size() const {
      return len_;
    }
    
    // 何も文字列を格納していない場合には true
    // Precond: None
    bool empty() const {
      return len_ == 0;
    }
    
    // 検索クエリの最大長を得る
    // Precond: None
    std::size_t max_query_length() const {
      return max_query_len_;
    }
    
    // 有効な状態であるか否か（ move されていないか）を得る
    // Precond: None
    bool valid() const {
      return suffix_.get() != 0 || this->empty();
    }
    
    // イテレータ
    // this->target() に対する，ソート済みの suffix array へのアクセスを得る
    // Precond: this->valid() == true
    typedef std::size_t           value_type;
    typedef value_type      *       iterator;
    typedef value_type const* const_iterator;
    
    // Returns: this->search( "", 0 ).first
    const_iterator begin() const {
      assert( this->valid() );
      return suffix_.get();
    }
    // Returns: this->search( "", 0 ).second
    const_iterator    end() const { return begin() + len_; }
    // Returns: begin()/end()
    const_iterator cbegin() const { return begin(); }
    const_iterator   cend() const { return end(); }
    
    
    // 検索
    
    // suffix array の中から，冒頭が [ q, q+n ) と一致する要素の range を検索する
    // Complexity : Ο( n * log( this->size() ) )
    // Precond  : this->valid() == true
    //            n <= this->max_query_length(),
    //            range [ q, q + n ) is valid, traits::length(q) <= n
    // Postcond : this->begin() <= result.first
    //            result.first  <= result.second
    //            result.second <= this->end()
    std::pair<const_iterator, const_iterator>
      search( charT const* q, std::size_t n ) const
    {
      assert( this->valid() );
      assert( n <= max_query_len_ );
      return equal_range_( s_, this->begin(), this->end(), q, n );
    }
    std::pair<const_iterator, const_iterator>
      search( charT const* q ) const
    {
      return this->search( q, traits::length(q) );
    }
    
    
   private:
    charT const* s_; std::size_t  len_;
    std::size_t max_query_len_;
    std::unique_ptr<value_type[]> suffix_;
    
    
    // 実装用関数群
    
    // ソートアルゴリズムを指定して suffix array を作る
    template< class Sort >
    static std::unique_ptr<value_type[]>
      make_suffix_array_(
        charT const* s, std::size_t len, std::size_t max_query_len, Sort sort )
    {
      // メモリ確保
      // len == 0 の場合はメモリ確保する必要はないが，それはレアケースであるため，
      // len をチェックするコストだけ逆にコスト増になりそうなので，チェックしない
      // メモリ確保した場合でも，別にエラーになることはない．
      std::unique_ptr<value_type[]> buf( new value_type[len] );
      
      // とりあえず順に埋めてから
      for( std::size_t i = 0; i < len; ++i ) {
        buf[i] = i;
      }
      
      // ソートアルゴリズムを呼ぶ
      sort( buf.get(), buf.get() + len, s, max_query_len );
      
      return buf;
    }
    
    // デフォルトのソートアルゴリズムで suffix array を作る
    static std::unique_ptr<value_type[]>
      make_suffix_array_( charT const* s, std::size_t len, std::size_t max_query_len )
    {
      return make_suffix_array_(
        s, len, max_query_len,
        []( iterator first, iterator last, charT const* t, std::size_t n ) {
          std::sort(
            first, last,
            [t,n]( std::size_t i, std::size_t j ){
              return traits::compare( t + i, t + j, n ) < 0;
            }
          );
        }
      );
    }
    
    // s に対応するソート済みの suffix array [ first, last ) の中から，
    // 冒頭 n 文字が q とマッチする最初の要素を指すイテレータを返す
    static const_iterator
      lower_bound_(
        charT const* s, const_iterator first, const_iterator last,
        charT const* q, std::size_t n
      )
    {
      for(;;) {
        std::size_t const len = last - first;
        if( len == 0 ){ return first; }
        
        std::size_t    const half   = len / 2;
        const_iterator const middle = first + half;
        
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
    static const_iterator
      upper_bound_(
        charT const* s, const_iterator first, const_iterator last,
        charT const* q, std::size_t n
      )
    {
      for(;;) {
        std::size_t const len = last - first;
        if( len == 0 ){ return first; }
        
        std::size_t    const half   = len / 2;
        const_iterator const middle = first + half;
        
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
    static std::pair<const_iterator, const_iterator>
      equal_range_(
        charT const* s, const_iterator first, const_iterator last,
        charT const* q, std::size_t n
      )
    {
      // [ first, last ) の幅を狭める
      for(;;)
      {
        std::size_t const len = last - first;
        if( len == 0 ){ return std::make_pair( first, last ); }
        
        std::size_t    const half   = len / 2;
        const_iterator const middle = first + half;
        
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
