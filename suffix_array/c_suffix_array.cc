#include "basic_suffix_array.hpp"

#include <iostream>

int main()
{
  pezzi::c_suffix_array sa( "KOTOKO" );
  auto const str = sa.c_str();
  
  for( auto const s : sa ) {
    std::cout << s << std::endl;
  }
  
  auto const found_ = sa.search( "KO" );
  std::vector<char const*> found( found_.first, found_.second );
  std::sort( found.begin(), found.end() );
  
  for( auto const pos : found ) {
    std::cout << ( pos - str ) << std::endl;
  }
}
