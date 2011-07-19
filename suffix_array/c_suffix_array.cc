#include "basic_suffix_array.hpp"

#include <iostream>
#include <boost/foreach.hpp>
#include <cstring>

int main( int argc, char* argv[] )
{
  // ����
  char const* const target_string = ( argc > 1 ) ? argv[1] : "KOTOKO";
  char const* const search_string = ( argc > 2 ) ? argv[2] : "KO";
  
  std::size_t const target_length = std::strlen(target_string);
  std::size_t const search_length = std::strlen(search_string);
  
  // suffix array �̍\�z
  pezzi::c_suffix_array const sa( target_string, target_length, search_length );
  
  // �_���v
  BOOST_FOREACH( auto const i, sa ) {
    std::cout << i << ", " << ( target_string + i ) << std::endl;
  }
  std::cout << std::endl;
  
  // ����
  auto const found_ = sa.search( search_string, search_length );
  
  // �������ʂ� vector �ɕό`�C�\�[�g
  std::vector<int> found( found_.first, found_.second );
  std::sort( found.begin(), found.end() );
  
  // ���ʂ̕\��
  BOOST_FOREACH( auto const i, found ) {
    std::cout << i << std::endl;
  }
}
