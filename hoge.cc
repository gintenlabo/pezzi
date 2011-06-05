// 適当に検証用に使う使い捨てコード
// C++ インタプリタ欲しいなー。無理だろうけど。
// このコメントはエンコードを UTF-8 にするために書いてるのです。

#include <random>
#include <ctime>
#include <iostream>
#include <algorithm>
#include <functional>

int main()
{
  std::uint32_t seed = static_cast<std::uint32_t>( std::time(0) );
  
  std::mt19937 gen( seed );
  
  std::size_t const teisu    = 10000;
  std::size_t const kohosha  = 20000;
  std::size_t const yukensha = teisu * teisu;
  
  std::uniform_int_distribution<> dist( 0, kohosha - 1 );
  
  std::vector<int> koho_list( kohosha, 0 );
  for( std::size_t i = 0; i < yukensha; ++i ) {
    std::size_t const vote = dist( gen );
    koho_list[vote] += 1;
  }
  
  std::sort( koho_list.begin(), koho_list.end(), std::greater<int>() );
  
  std::cout << "最大得票： " << koho_list[0] << std::endl;
  std::cout << "当選ライン： " << koho_list[teisu-1] << std::endl;
  std::cout << "落選ライン： " << koho_list[teisu] << std::endl;
  std::cout << "最低得票： " << koho_list[kohosha-1] << std::endl;
}
