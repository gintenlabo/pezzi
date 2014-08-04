// std::min/std::max のテスト

struct X
{
  int value;
};

#include <algorithm>
#include <iostream>

int main()
{
  X const x1 = {1}, x2 = {2};
  
  auto comp = [] ( X const& a, X const& b ) {
    return a.value < b.value;
  };
  
  std::cout << std::min( x1, x2, comp ).value << std::endl;
  std::cout << std::max( x1, x2, comp ).value << std::endl;
  
}
