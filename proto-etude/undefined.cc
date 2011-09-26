// undefined : Haskell が羨ましかったので

#include <cassert>
#include <stdexcept>
#include <utility>

namespace proto_etude
{
  /*
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
  */
  
  // あらゆる型に評価できるが，実際に評価されると例外を投げる型
  struct undefined_t
  {
    // 任意の型への変換
    template< class T >
    operator T&&() const {
      throw std::logic_error( "etude::undefined must not be used!" );
    }
    
    // 任意の関数適用
    template< class... Types >
    undefined_t operator()( Args&&... ) const {
      return {};
    }
    
  };
  undefined_t const undefined = {};
  
  
  // 例外を投げる関数．
  // throw との違いは，戻り値をあらゆる型として評価できる点
  
  // 例外オブジェクトを指定して例外を投げる
  template< class Exception, class... Args >
  inline undefined_t error( Args&&... args )
  {
    throw Exception( std::forward<Args>(args)... );
  }
  
  // std::logic_error を投げる
  template< class What >
  inline undefined_t error( What && what ) {
    throw std::logic_error( std::forward<What>( what ) );
  }
  
  // デフォルトのエラーメッセージでエラーを投げる
  inline undefined_t error() {
    throw std::logic_error( "etude::error() must not be used!" );
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
  
  try {
    std::cout <<  dereference( (int*)0 ) << std::endl;
  }
  catch( std::runtime_error& ) {
    std::cout << "OK.\n";
  }
}
