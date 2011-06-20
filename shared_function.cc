// shared_function : shared_ptr で関数を保持した function の変形
// etude 候補

#include <utility>
#include <type_traits>
#include <memory>
#include <functional>

namespace pezzi
{
  // プロトタイプ
  
  // 基底クラス
  template< class Signature >
  struct shared_function_base;  // never defined
  
  // helper class
  template< class Signature, class F >
  struct shared_function_body;  // never defined
  
  // 本体
  template< class Signature >
  struct shared_function;       // never defined
  
  
  // 実装
  
  // 基底クラス
  template< class R, class... Args >
  struct shared_function_base<R (Args...)>
  {
    shared_function_base() {}
    virtual ~shared_function_base() {}
    
    shared_function_base( shared_function_base const& ) = delete;
    void operator=( shared_function_base const& ) = delete;
    
    // 本体，これを書き換える
    virtual R invoke( Args... ) = 0;
    
  };
  
  // 派生クラス
  template< class R, class... Args, class F >
  struct shared_function_body<R(Args...), F>
    : shared_function_base<R(Args...)>
  {
    template< class... As,
      class = typename std::enable_if<
        std::is_constructible<F, As...>::value
      >::type
    >
    explicit shared_function_body( As&&... as )
      : f_( std::forward<As>(as)... ) {}
    
    R invoke( Args... args ) {
      return static_cast<R>( f_( std::forward<Args>(args)... ) );
    }
    
    F &      get()       { return f_; }
    F const& get() const { return f_; }
    
   private:
    F f_;
    
  };
  
  // 本体
  template< class R, class... Args >
  class shared_function<R (Args...)>
  {
    typedef shared_function self_type;
    
   public:
    typedef R signature( Args... );
    typedef signature function_type;
    typedef R result_type;
    
    typedef shared_function_base<signature> base_type;
    typedef std::shared_ptr<base_type> pointer_type;
    
    
    // ctors
    shared_function() : p_() {}
    
    template< class F >
    explicit shared_function( F && f )
      : p_( make_body_( std::forward<F>(f) ) ) {}
    
    template< class F, class... As >
    explicit shared_function( F && f, As&&... as )
      : p_( make_body_( std::bind( std::forward<F>(f), std::forward<As>(as)... ) ) ) {}
    
    // invoke function
    R operator()( Args... args ) const
    {
      if( !p_ ){
        throw std::bad_function_call();
      }
      return p_->invoke( std::forward<Args>(args)... );
    }
    
    // modifiers
    void swap( self_type& other ) noexcept {
      p_.swap( other.p_ );
    }
    friend void swap( self_type& one, self_type& another ) noexcept {
      one.swap( another );
    }
    
    // observers
    explicit operator bool() const {
      return static_cast<bool>(p_);
    }
    
    pointer_type const& get_pointer() const {
      return p_;
    }
    pointer_type release_pointer() {
      return std::move( p_ );
    }
    
   private:
    pointer_type p_;
    
    template< class F >
    static pointer_type make_body_( F && f ) {
      typedef typename std::decay<F>::type FD;
      typedef shared_function_body<signature, FD> body_type;
      return std::make_shared<body_type>( std::forward<F>(f) );
    }
    
  };
  
}

#include <iostream>
#include <boost/noncopyable.hpp>

int f( int x, int y ) {
  return x * x + y * y;
}

struct X
  : private boost::noncopyable
{
  void foo() {
    std::cout << "X::foo\n";
  }
};

pezzi::shared_function<void()> createX()
{
  auto p = std::make_shared<X>();
  return pezzi::shared_function<void()>( [p]{ p->foo(); } );
}

int main()
{
  auto pf = createX();
  pf();
}
