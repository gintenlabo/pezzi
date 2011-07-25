// std::allocate_shared を hack して，
// 実行時に確保するメモリ領域の大きさを指定できるようにする
#include <memory>
#include <cassert>
#include <iostream>

struct buffer_info_
{
  void*       addr;
  std::size_t size;
};

// C++0x を期待
template< class T, class AlignAs = T >
struct hacked_allocator
{
  // GCC-4.6 はこれを要求するみたい
  template< class U >
  struct rebind { typedef hacked_allocator<U, AlignAs> other; };
  
  template< class U, class A_ >
  friend class hacked_allocator;
  
  // construct
  explicit hacked_allocator( buffer_info_& info )
    : p( &info ) {}
  
  template< class U, class A_ >
  hacked_allocator( hacked_allocator<U, A_> const& src )
    : p( src.p ) {}
  
  
  T* allocate( std::size_t n )
  {
    assert( p != 0 );
    
    std::ptrdiff_t const buf_offset = adjust_align_( n * sizeof(T) );
    void* const buf = ::operator new ( buf_offset + p->size );
    p->addr = static_cast<char*>(buf) + buf_offset;
    p = 0;
    
    std::cout << "allocated:   " << buf << std::endl;
    
    return static_cast<T*>(buf);
  }
  void deallocate( T* p, std::size_t ) {
    void* const addr = p;
    ::operator delete( addr );
    std::cout << "deallocated: " << addr << std::endl;
  }
  
 private:
  buffer_info_* p;
  
  static std::size_t adjust_align_( std::size_t size )
  {
    return ( size + alignof(AlignAs) - 1 ) / alignof(AlignAs) * alignof(AlignAs);
  }
  
};

// T 型のオブジェクトとバッファを同時に構築する（ Alignment 指定あり）
template< class T, class AlignAs, class... Args >
inline auto make_shared_with_aligned_extra_buffer( std::size_t buffer_size, Args&&... args )
  -> std::pair< std::shared_ptr<T>, void* >
{
  buffer_info_ rs = { 0, buffer_size };
  
  auto p = std::allocate_shared<T>(
    hacked_allocator<T>(rs), std::forward<Args>(args)...
  );
  void* const buf = rs.addr;
  assert( buf != 0 );
  
  std::cout << "p.get(): " << static_cast<void*>(p.get()) << std::endl;
  std::cout << "buf:     " << buf << std::endl;
  
  return std::make_pair( std::move(p), buf );
  
}

// T 型のオブジェクトとバッファを同時に構築する（ Alignment 無指定）
// 今回は使わないが，これもあってよいだろう
template< class T, class... Args >
inline auto make_shared_with_extra_buffer( std::size_t buffer_size, Args&&... args )
  -> std::pair< std::shared_ptr<T>, void* >
{
  return make_shared_with_aligned_extra_buffer<T>(
    buffer_size, std::forward<Args>(args)...
  );
}

// この応用例として，動的配列を make_shared する

// …の前に，まず実装用の部品を製作

// 配列に対する RAII を扱うクラス
// http://d.hatena.ne.jp/y-hamigaki/20071217/1197892499 の
// scoped_placement_array を参考に
#include <boost/noncopyable.hpp>

template< class T >
struct placement_array
  : private boost::noncopyable
{
  // construct
  placement_array() noexcept
    : n_(0) {}  // n_ が 0 なら， p_ は何でもいい
  
  // destruct
  ~placement_array() {
    this->reset();
  }
  
  // move
  placement_array( placement_array && src ) noexcept
    : p_( src.p_ ), n_( src.n_ )
  {
    src.n_ = 0;
  }
  
  // swap
  void swap( placement_array& other ) noexcept {
    std::swap( p_, other.p_ );
    std::swap( n_, other.n_ );
  }
  
  // move assignment
  placement_array& operator=( placement_array && src ) noexcept {
    this->swap( src );
    return *this;
  }
  
  // reset
  void reset() noexcept {
    // T の dtor は例外を投げないとする
    std::size_t n = n_;
    T* p = p_;
    
    while( n > 0 ) {
      p[--n].~T();
    }
    
    // p_ は変更する必要なし
    n_ = 0;
  }
  
  // construct
  void construct( T* p, std::size_t n )
  {
    this->reset();
    
    this->p_ = p;
    
    for( ; n_ < n; ++n_ ) {
      ::new( p + n_ ) T();
    }
  }
  
  // その他のメンバは面倒なので後で実装する（しないかも）
  
 private:
  T*          p_;
  std::size_t n_;
  
};

// 配列を構築する
template< class T >
inline std::shared_ptr<T> make_shared_array( std::size_t n )
{
  typedef placement_array<T> body_type;
  auto p_ = make_shared_with_aligned_extra_buffer<body_type, T>( n * sizeof(T) );
  
  auto& p = p_.first;
  T* const buf = static_cast<T*>( p_.second );
  
  p->construct( buf, n );
  
  return std::shared_ptr<T>( std::move(p), buf );
  
}


// テスト
#include <iostream>
#include <stdexcept>

#include <random>
#include <ctime>

// [ 0, n ) の乱数を生成
int gen_uniform_int( int n )
{
  static std::mt19937 gen( static_cast<uint32_t>( std::time(0) ) );
  std::uniform_int_distribution<int> dist( 0, n-1 );
  
  return dist(gen);
  
}

struct Hoge
{
  int x;
  
  Hoge()
    : x()
  {
    // 例外安全性チェック
    if( gen_uniform_int(5) == 0 ) {
      std::cout << "\n ! ! ! EXCEPTION WAS RAISED ! ! ! \n\n";
      throw std::runtime_error("exception!");
    }
    std::cout << "Hoge::Hoge()  <" << this << ">\n";
  }
  
  ~Hoge()
  {
    std::cout << "Hoge::~Hoge() <" << this << ">\n";
  }
  
};


int main()
{
  try
  {
    auto p0 = make_shared_array<Hoge>(0);
    std::cout << std::endl;
    
    std::cout << p0 << std::endl;
    std::cout << std::endl;
    
    auto p1 = make_shared_array<Hoge>(5);
    std::cout << std::endl;
    
    std::cout << p1 << std::endl;
    std::cout << std::endl;
  }
  catch( std::exception& e )
  {
    std::cout << "\nterminated.\n";
    return 1;
  }
}
