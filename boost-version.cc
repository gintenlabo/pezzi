// Boost のバージョンを表示するだけのコード

#include <boost/version.hpp>
#include <iostream>

int main()
{
  int const major = BOOST_VERSION / 100000,
             minor = BOOST_VERSION / 100 % 1000,
             patch = BOOST_VERSION % 100;
  
  std::cout << major << '.' << minor << '.' << patch << '\n';
  
}
