// あるビット列の乱雑さを求める

#include <bitset>

// とりあえず自己相関の重み付け平均で．
// まず間違いなく，もっとよい方法はありそう．
template< std::size_t N >
inline double calc_randomness( std::bitset<N> const& data )
{
  double sum = 0;
  double w = 0;
  
  for( std::size_t i = 0; i < N; ++i ) {
    if( data[i] ) {
      sum += 1;
    }
    else {
      sum -= 1;
    }
  }
  sum = sum * sum;
  w += N * N;
  
  for( std::size_t d = 1; d < N; ++d )
  {
    int Rd = 0;
    
    for( std::size_t i = 0, j = i + d; j < N; ++i, ++j ) {
      if( data[i] == data[j] ) {
        Rd += 1;
      }
      else {
        Rd -= 1;
      }
    }
    
    sum += double(Rd) * double(Rd);
    w += (N-d) * (N-d);
  }
  
  return sum / w;
}

#include <iostream>
#include <algorithm>
#include <random>
#include <ctime>

int main()
{
  std::size_t const n = 128;
  std::uint32_t const seed = std::time(0);
  std::mt19937 gen( seed );
  
  std::bitset<n> a;
  std::cout << calc_randomness(a) << std::endl;
  
  a[gen() % n] = 1;
  std::cout << calc_randomness(a) << std::endl;
  
  for( std::size_t i = 1; i < n; i += 2 ) {
    a[i] = 1;
  }
  std::cout << calc_randomness(a) << std::endl;
  
  
  for( double p = 0.1; p < 0.91; p += 0.1 ) {
    std::bernoulli_distribution dist( p );
    for( std::size_t i = 0; i < n; ++i ) {
      a[i] = dist(gen);
    }
    std::cout << p << ": " << calc_randomness(a) << std::endl;
  }
}
