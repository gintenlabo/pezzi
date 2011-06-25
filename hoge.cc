// 適当に検証用に使う使い捨てコード
// C++ インタプリタ欲しいなー。無理だろうけど。
// このコメントはエンコードを UTF-8 にするために書いてるのです。

#include <memory>
#include <functional>

#define STATIC_ASSERT(...) static_assert( __VA_ARGS__, #__VA_ARGS__ )

#include <boost/noncopyable.hpp>
#include <iostream>

struct X
  : private boost::noncopyable
{
  explicit X( int m_ )
    : member( m_ ) {}
  
  ~X() {
    std::cout << "X::~X()\n";
  }
  
  void foo() {
    std::cout << "The member is " << member << ".\n";
  }
  
 private:
  int member;
  
};

std::shared_ptr<std::function<void()>> make_some_function()
{
  struct impl_
    : X
  {
    std::function<void()> f;
    
    impl_( int m_ )
      : X(m_) {}
    
  };
  
  auto  p = std::make_shared<impl_>( 42 );
  auto& f = p->f;
  auto& x = static_cast<X&>(*p);
  
  f = [&x]{ x.foo(); };
  
  return std::shared_ptr<std::function<void()>>( p, &f );
}


int main()
{
  auto pf = make_some_function();
  (*pf)();
}
