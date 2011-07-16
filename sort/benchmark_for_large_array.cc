// 大きな配列に対する各種アルゴリズムのベンチマーク
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
#include "comb_sort.hpp"

#include <boost/progress.hpp>
#include <random>
#include <cmath>
#include <cassert>

template< class Sort >
inline void benchmark( std::vector<int> vec, Sort sort )
{
  {
    // 測定性能
    boost::progress_timer t;
    sort( vec );
  }
  
  // チェック
  if( !std::is_sorted( vec.begin(), vec.end() ) ){
    assert( !"sorting algorithm is wrong." );
  }
}

#include <unistd.h> // for getopt
#include <boost/lexical_cast.hpp>
#include <boost/format.hpp>

int main( int argc, char* argv[] )
{
  std::size_t n_ = 5000000;
  
  {
    int opt;
    while( ( opt = getopt( argc, argv, "n:m:" ) ) != -1 ) {
      try {
        switch( opt ) {
         case 'n':
          n_ = boost::lexical_cast<int>( optarg );
          break;
         default:
          std::cerr <<
            boost::format( "usage: %s [ -n size ]\n" ) % argv[0];
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
  
  std::mt19937 gen( static_cast<std::uint32_t>( std::time(0) ) );
  std::normal_distribution<> dist( 0.0, std::sqrt(n) );
  
  auto const vec = generate_vector<int>( n, [&](){ return int( round( dist(gen) ) ); } );
  
  // 一応，先頭を軽く表示しておく
  for( std::size_t i = 0; i < 10; ++i ) {
    std::cout << vec[i] << ' ';
  }
  std::cout << "\n\n";
  
  // std::sort
  std::cout << "std::sort...\n";
  benchmark( vec, []( std::vector<int>& vec ){
    std::sort( vec.begin(), vec.end() );
  } );
  // ヒープソート
  std::cout << "Heap Sort...\n";
  benchmark( vec, []( std::vector<int>& vec ){
    std::make_heap( vec.begin(), vec.end() );
    std::sort_heap( vec.begin(), vec.end() );
  } );
  // コムソート
  std::cout << "Comb Sort...\n";
  benchmark( vec, []( std::vector<int>& vec ){
    pezzi::comb_sort( vec.begin(), vec.end() );
  } );
}
