#include "basic_suffix_array.hpp"

#include <iostream>
#include <boost/foreach.hpp>

int main()
{
  pezzi::c_suffix_array sa( "KOTOKO" );
  auto const str = sa.c_str();
  
  BOOST_FOREACH( auto const i, sa ) {
    std::cout << i << ", " << ( str + i ) << std::endl;
  }
  
  auto const found_ = sa.search( "KO" );
  std::vector<int> found( found_.first, found_.second );
  std::sort( found.begin(), found.end() );
  
  BOOST_FOREACH( auto const i, found ) {
    std::cout << i << std::endl;
  }
}
