// boost::make_shared を boost::optional を用いて実装する

#include <boost/optional.hpp>
#include <boost/shared_ptr.hpp>

namespace pezzi
{
  // T 型のストレージつき deleter （実装用）
  template< class T >
  struct storage_deleter_
  {
    storage_deleter_() {}
    
    // コピーはコピーしない
    storage_deleter_( storage_deleter_ const& ) {}
    
    // 初期化
    template< class InPlace >
    void initialize( InPlace const& x ) {
      x_ = x;
    }
    
    // アドレス取得
    T *      get()       { return x_.get_ptr(); }
    T const* get() const { return x_.get_ptr(); }
    
    // operator() は optional を破棄
    void operator()( T* ) {
      x_ = boost::none;
    }
    
   private:
    boost::optional<T> x_;
    
    // 再代入はなし
    void operator=( storage_deleter_ const& );
    
  };
  
  template< class T, class InPlace >
  boost::shared_ptr<T> make_shared_from_in_place( InPlace const& x )
  {
    // 仮の shared_ptr を作って参照カウントを作成し
    boost::shared_ptr<T> pt( static_cast<T*>(0), storage_deleter_<T>() );
    // 参照カウント内のデリータのアドレスを取得する
    storage_deleter_<T> &storage = *boost::get_deleter< storage_deleter_<T> >(pt);
    
    // デリータ内に用意された領域にオブジェクトを構築
    storage.initialize(x);
    
    // 構築されたオブジェクトのアドレスと最初に作った参照カウントから、
    // 改めて shared_ptr を制作する
    // 実際には enable_shared_from_this 周りの処理が必要
    return boost::shared_ptr<T>( pt, storage.get() );
    
  }
  
}

#include <boost/noncopyable.hpp>

// noncopyable なクラス
struct Hoge
  : private boost::noncopyable
{
  int i;
  
  explicit Hoge( int i_ )
    : i( i_ ) {}
  
};

#include <iostream>
#include <boost/utility/in_place_factory.hpp>

int main()
{
  boost::shared_ptr<Hoge> p = pezzi::make_shared_from_in_place<Hoge>( boost::in_place(0) );
  std::cout << p->i << std::endl;
}
