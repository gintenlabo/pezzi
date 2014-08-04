// http://cpplover.blogspot.com/2011/06/multiplelockguard.html
// の改良

#include <tuple>
#include <memory>
#include <mutex>

// 補助関数． 受け取った mutex を adopt_lock する
template< class Mutex >
std::unique_lock<Mutex> adopt_unique_lock( Mutex& m )
{
  return std::unique_lock<Mutex>( m, std::adopt_lock );
}

// クラスではなく関数にする（型推論しやすいよう）
template< class... Args,
  class Result = std::tuple<std::unique_lock<Args>...>
>
Result make_lock_guard( Args&... args )
{
  std::lock( args... );
  return Result( adopt_unique_lock(args)... );
}


std::mutex m1, m2, m3;

int main()
{
  auto lock = make_lock_guard( m1, m2, m3 );
}
