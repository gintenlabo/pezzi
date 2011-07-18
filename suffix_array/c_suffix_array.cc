#include "basic_suffix_array.hpp"

#include <iostream>
#include <boost/foreach.hpp>

int main( int argc, char* argv[] )
{
  // 入力
  char const* const target_string = ( argc > 1 ) ? argv[1] : "KOTOKO";
  char const* const search_string = ( argc > 2 ) ? argv[2] : "KO";
  
  // suffix array の構築
  pezzi::c_suffix_array sa( target_string );
  
  // ダンプ
  BOOST_FOREACH( auto const i, sa ) {
    std::cout << i << ", " << ( target_string + i ) << std::endl;
  }
  std::cout << std::endl;
  
  // 検索
  auto const found_ = sa.search( search_string );
  
  // 検索結果を vector に変形，ソート
  std::vector<int> found( found_.first, found_.second );
  std::sort( found.begin(), found.end() );
  
  // 結果の表示
  BOOST_FOREACH( auto const i, found ) {
    std::cout << i << std::endl;
  }
}
