// 小さいな配列に対する各種アルゴリズムのベンチマーク
#include <vector>

// 関数から vector を生成
template< class T, class F >
inline std::vector<T> generate_vector( std::size_t n, F f )
{
  std::vector<T> vec; vec.reserve(n);
  
  for( std::size_t i = 0; i < n; ++i ) {
    vec.push_back( f() );
  }
  
  return vec;
}

#include <algorithm>
#include "bubble_sort.hpp"
#include "shaker_sort.hpp"
#include "insertion_sort.hpp"

#include <boost/progress.hpp>
#include <random>
#include <cmath>
#include <cassert>

template< class Sort >
inline void benchmark( std::vector<int> vec, std::size_t m, Sort sort )
{
  std::size_t const n = vec.size();
  
  {
    // 測定性能
    boost::progress_timer t;
    
    for( int i = 0; i + m <= n; i += m ) {
      sort( &vec[i], &vec[i+m] );
    }
  }
  
  // チェック
  for( int i = 0; i + m <= n; i += m ) {
    assert( std::is_sorted( &vec[i], &vec[i+m] ) );
  }
}

#include <unistd.h> // for getopt
#include <boost/lexical_cast.hpp>
#include <boost/format.hpp>

int main( int argc, char* argv[] )
{
  std::size_t n_ = 5000000;
  std::size_t m_ = 5;
  
  {
    int opt;
    while( ( opt = getopt( argc, argv, "n:m:" ) ) != -1 ) {
      try {
        switch( opt ) {
         case 'n':
          n_ = boost::lexical_cast<int>( optarg );
          break;
         case 'm':
          m_ = boost::lexical_cast<int>( optarg );
          break;
         default:
          std::cerr <<
            boost::format( "usage: %s [ -n all-size ] [ -m seg-size ]\n" ) % argv[0];
          return 1;
        }
      } catch( boost::bad_lexical_cast& ) {
          std::cerr <<
            boost::format( "given option '%s' is not an integer.\n" ) % optarg;
          return 1;
      }
    }
  }
  
  std::size_t const n = n_;
  std::size_t const m = m_;
  
  std::mt19937 gen( static_cast<std::uint32_t>( std::time(0) ) );
  std::normal_distribution<> dist( 0.0, std::sqrt(m) );
  
  auto const vec = generate_vector<int>( n, [&](){ return int( round( dist(gen) ) ); } );
  
  // 一応，先頭を軽く表示しておく
  for( std::size_t i = 0; i < m; ++i ) {
    std::cout << vec[i] << ' ';
  }
  std::cout << "\n\n";
  
  // std::sort
  std::cout << "std::sort...\n";
  benchmark( vec, m, []( int* first, int* last ){ std::sort( first, last ); } );
  // バブルソート
  std::cout << "Bubble Sort...\n";
  benchmark( vec, m, []( int* first, int* last ){ pezzi::bubble_sort( first, last ); } );
  // シェイカーソート
  std::cout << "Shaker Sort...\n";
  benchmark( vec, m, []( int* first, int* last ){ pezzi::shaker_sort( first, last ); } );
  // 挿入ソート
  std::cout << "Insertion Sort...\n";
  benchmark( vec, m, []( int* first, int* last ){ pezzi::insertion_sort( first, last ); } );
}
