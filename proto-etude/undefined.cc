// undefined : Haskell 縺ｮ繧｢繝ｬ

#include <cassert>
#include <stdexcept>
#include <cassert>
#include <utility>

namespace proto_etude
{
  // あらゆる型に評価できるが，実際に評価されうる場合にはコンパイルエラーとなる型
  struct undefined_t
  {
    template<typename T>
    struct protector_ {
      static const bool stop_ = false;
      static T&& delegate_(); // never defined
    };
    
    template< class T >
    operator T&&() const {
      static_assert( protector_<T>::stop_, "etude::undefined must not be used!" );
      return protector_<T>::delegate_();
    }
    
  };
  undefined_t const undefined = {};
  
  
  // あらゆる型に評価できるが，実際に評価されると例外を投げる型
  struct error_t
  {
    template< class T >
    operator T&&() const {
      throw std::logic_error( "etude::error_t::operator T&&() was used" );
    }
  };
  
  // 評価されると例外を投げる．
  // 戻り値はあらゆる型として評価可能
  inline error_t error( std::string const& what ) {
    throw std::logic_error( what );
  }
  inline error_t error( char const* what ) {
    throw std::logic_error( what );
  }
  
  // 引数省略版
  inline error_t error() {
    return proto_etude::error( "etude::error() was used." );
  }
  
  // 例外オブジェクト指定版
  template< class Exception, class... Args >
  inline error_t error( Args&&... args )
  {
    throw Exception( std::forward<Args>(args)... );
  }
  
}

#include <iostream>

template< class T, class... Args >
typename std::decay<T>::type head( T && x, Args&&... ) {
  return std::forward<T>(x);
}

template< class T >
inline T& dereference( T* p ) {
  return p ? *p : proto_etude::error<std::runtime_error>( "ぬるぽ" );
}


int main()
{
  auto x = head( 1, proto_etude::undefined ); // OK.
  (void)x;
  
  constexpr int i = true ? 0 : proto_etude::error();
  std::cout <<  dereference(&i) << std::endl;
  // std::cout <<  dereference( (int*)0 ) << std::endl;
  
}
