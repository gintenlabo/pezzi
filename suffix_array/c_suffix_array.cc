#include "basic_suffix_array.hpp"

#include <iostream>

int main()
{
  pezzi::c_suffix_array sa( "KOTOKO" );
  auto const str = sa.c_str();
  
  for( auto const i : sa ) {
    std::cout << i << ", " << ( str + i ) << std::endl;
  }
  
  auto const found_ = sa.search( "KO" );
  std::vector<int> found( found_.first, found_.second );
  std::sort( found.begin(), found.end() );
  
  for( auto const i : found ) {
    std::cout << i << std::endl;
  }
}
