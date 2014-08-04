// Boost.Pool の使い方

#include <boost/pool/pool_alloc.hpp>
#include <boost/progress.hpp>
#include <boost/make_shared.hpp>
#include <unistd.h> // for getopt
#include <stdexcept>
#include <boost/lexical_cast.hpp>
#include <boost/format.hpp>

int main( int argc, char* argv[] )
{
  long n = 1000000;
  
  // オプション解析（ループ回数を指定できるよう）
  try {
    char ch;
    while( ( ch = getopt( argc, argv, "n:" ) ) != -1 )
    {
      switch(ch)
      {
       case 'n':
        n = boost::lexical_cast<long>( optarg );
        if( n <= 0 ) {
          throw std::runtime_error("invalid option");
        }
        break;
        
       default:
        throw std::runtime_error("unknown option");
        
      }
    }
  }
  catch( std::exception& ) {
    std::cerr << boost::format("usage: %1% [-n N]\n") % argv[0];
    return -1;
  }
  
  // 測定
  {
    std::cout << "non-pool:\n";
    boost::progress_timer t;
    
    for( long i = 0; i < n; ++i ) {
      boost::shared_ptr<int> p = boost::make_shared<int>();
      (void)p;
    }
  }
  {
    std::cout << "pool:\n";
    boost::progress_timer t;
    
    boost::fast_pool_allocator<int> alloc;
    for( long i = 0; i < n; ++i ) {
      boost::shared_ptr<int> p = boost::allocate_shared<int>( alloc );
      (void)p;
    }
  }
}
