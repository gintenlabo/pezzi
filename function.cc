// std::function の実装の確認

#include <memory>
#include <iostream>

// 「やかましい」アロケータ
template< class T >
struct noisy_allocator
  : private std::allocator<T>
{
  typedef std::allocator<T> base_type;
  base_type &      base()       { return *this; }
  base_type const& base() const { return *this; }
  
  typedef typename base_type::value_type           value_type;
  typedef typename base_type::size_type             size_type;
  typedef typename base_type::difference_type difference_type;
  
  typedef typename base_type::pointer                 pointer;
  typedef typename base_type::const_pointer     const_pointer;
  typedef typename base_type::reference             reference;
  typedef typename base_type::const_reference const_reference;
  
  using base_type::address;
  using base_type::max_size;
  using base_type::construct;
  using base_type::destroy;
  
  
  // rebind
  template< class U >
  struct rebind { typedef noisy_allocator<U> other; };
  
  
  // construct
  noisy_allocator() = default;
  
  template< class U >
  noisy_allocator( noisy_allocator<U> const& src )
    : base_type( src.base() ) {}
  
  
  T* allocate( std::size_t n ) {
    T* const p = base().allocate(n);
    std::cout << "allocated:   " << static_cast<void*>(p) << "\n";
    return p;
  }
  void deallocate( T* p, std::size_t n ) {
    base().deallocate( p, n );
    std::cout << "deallocated: " << static_cast<void*>(p) << "\n";
  }
  
  
  friend bool operator==( noisy_allocator const&, noisy_allocator const& ) {
    return true;
  }
  friend bool operator!=( noisy_allocator const&, noisy_allocator const& ) {
    return false;
  }
  
};


#include <functional>
#include <vector>
#include <boost/progress.hpp>

int main()
{
  std::size_t const n = 10000000;
  
  auto p = std::make_shared<int>(42);
  std::function<int()> f1 = [&p](){ return p ? *p : 0; };
  std::function<int()> f2 = [ p](){ return p ? *p : 0; };
  
  // n 回 std::shared_ptr をコピー
  {
    std::vector<std::shared_ptr<int>> vec;
    vec.reserve(n);
    
    boost::progress_timer t;
    
    for( std::size_t i = 0; i < n; ++i ) {
      vec.push_back(p);
    }
    
    vec.clear();
  }
  
  // n 回 std::function をコピー（参照キャプチャ）
  {
    std::vector<std::function<int()>> vec;
    vec.reserve(n);
    
    boost::progress_timer t;
    
    for( std::size_t i = 0; i < n; ++i ) {
      vec.push_back(f1);
    }
    
    vec.clear();
  }
  
  // n 回 std::function をコピー（値キャプチャ）
  {
    std::vector<std::function<int()>> vec;
    vec.reserve(n);
    
    boost::progress_timer t;
    
    for( std::size_t i = 0; i < n; ++i ) {
      vec.push_back(f2);
    }
    
    vec.clear();
  }
}
