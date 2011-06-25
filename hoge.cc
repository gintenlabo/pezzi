// 適当に検証用に使う使い捨てコード
// C++ インタプリタ欲しいなー。無理だろうけど。
// このコメントはエンコードを UTF-8 にするために書いてるのです。

#include <iostream>

struct X
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

#include <memory>
#include <functional>
#include <utility>
#include <tuple>

std::shared_ptr<std::function<void()>> make_some_function()
{
  typedef std::function<void()> function_t;
  typedef std::pair<X, function_t> pair_t;
  
  auto  p = std::make_shared<pair_t>( std::piecewise_construct,
    std::make_tuple( 42 ), std::make_tuple()
  );
  auto& x = p->first;
  auto& f = p->second;
  
  f = [&x]{ x.foo(); };
  
  return std::shared_ptr<std::function<void()>>( p, &f );
}


int main()
{
  auto pf = make_some_function();
  (*pf)();
}
