// constexpr な std::next と std::prev
// noexcept 対応はしない（未定義動作が起き得るので）
#include <utility>
#include <type_traits>
#include <iterator>

namespace pezzi
{
  // constexpr な forward
  template< class T >
  constexpr T&& forward( typename std::remove_reference<T>::type& x ) noexcept {
    return static_cast<T&&>(x);
  }
  // rvalue は forward 不可にする
  // int x; std::forward<unsigend>(x); みたいな例を ill-formed にするため
  template< class T >
  void forward( typename std::remove_reference<T>::type&& x ) = delete;
  
  // random access iter かつ literal type なら iter + 1 を返す
  template<
    class Iter_,
    class Iter = typename std::decay<Iter_>::type,
    class = typename std::enable_if<std::is_literal_type<Iter>{}>::type
  >
  constexpr Iter next_( Iter_ && iter, std::random_access_iterator_tag* ) {
    return pezzi::forward<Iter_>(iter) + 1;
  }
  
  // それ以外なら ++iter を返す
  template< class Iter  >
  Iter next_( Iter iter, void* ) {
    ++iter; return iter;  // move は要らない
  }
  
  // 本体
  template< class Iter_,
    class Iter = typename std::decay<Iter_>::type,
    class Category = typename std::iterator_traits<Iter>::iterator_category
  >
  constexpr Iter next( Iter_ && iter ) {
    return pezzi::next_( pezzi::forward<Iter_>(iter), static_cast<Category*>(0) );
  }
  
} // namespace pezzi


#include <deque>
#include <list>
#include <iostream>

int main()
{
  constexpr int a[3] = { 1, 2, 3 };
  /*constexpr*/ auto it0 = pezzi::next( a+0 );
  std::cout << *it0 << std::endl;
  
  std::deque<int> ls1 = { 1, 2, 3 };
  auto it1 = pezzi::next( ls1.begin() );
  std::cout << *it1 << std::endl;
  
  std::list<int> ls2 = { 1, 2, 3 };
  auto it2 = pezzi::next( ls2.begin() );
  std::cout << *it2 << std::endl;
  
}
