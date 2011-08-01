// undefined : Haskell のアレ

#include <cassert>
#include <stdexcept>
#include <utility>

namespace proto_etude
{
  struct undefined_value_evaluation
    : std::logic_error
  {
    undefined_value_evaluation()
      : std::logic_error( "etude::undefined has been used." ) {}
    
  };
  
  struct undefined_t
  {
    template< class... Args >
    undefined_t operator()( Args&&... ) const {
      throw undefined_value_evaluation();
    }
    
    template< class T >
    operator T&&() const {
      throw undefined_value_evaluation();
    }
    
  };
  
  undefined_t const undefined = {};
  
  
  inline undefined_t error()
  {
    return undefined();
    // throw undefined_value_evaluation();
  }
  
  inline undefined_t error( std::string const& what ) {
    throw std::logic_error( what );
  }
  inline undefined_t error( char const* what ) {
    throw std::logic_error( what );
  }
  
  template< class Exception, class... Args >
  inline undefined_t error( Args&&... args )
  {
    throw Exception( std::forward<Args>(args)... );
  }
  
}

#include <iostream>

template< class T >
inline T& dereference( T* p ) {
  return p ? *p : proto_etude::error<std::runtime_error>( "ぬるぽ" );
}

int main()
{
  // int i = 0;
  // auto& x = false ? i : proto_etude::undefined;
  // (void)x;
  
  constexpr int i = true ? 0 : proto_etude::undefined;
  std::cout <<  dereference(&i) << std::endl;
  std::cout <<  dereference( (int*)0 ) << std::endl;
  
}
