// GCC のバージョンを表示するだけのコード

#include <iostream>

int main()
{
  int const major = __GNUC__,
            minor = __GNUC_MINOR__,
            patch = __GNUC_PATCHLEVEL__;
  
  std::cout << major << '.' << minor << '.' << patch << '\n';
  
}
