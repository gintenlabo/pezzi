// http://cpplover.blogspot.com/2011/06/multiplelockguard.html
// の改良

#include <tuple>
#include <memory>
#include <mutex>

template< class Mutex >
struct unlock
{
  void operator()( Mutex* m ) const {
    m->unlock();
  }
};

// クラスではなく関数にする（型推論しやすいよう）
template< class... Args,
  class Result = std::tuple<std::unique_ptr<Args, unlock<Args>>...>
>
Result make_lock_guard( Args&... args )
{
  std::lock( args... );
  return Result( std::addressof(args)... );
}


std::mutex m1, m2, m3;

int main()
{
  auto lock = make_lock_guard( m1, m2, m3 );
}
