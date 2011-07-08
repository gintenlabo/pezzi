namespace pezzi
{
  struct addressof_helper_ {};
  
  template< class T >
  T* addressof( T& x )
  {
    return reinterpret_cast<T*>(
      const_cast<addressof_helper_*>(
        &reinterpret_cast<addressof_helper_ const volatile &>(x)
      )
    );
  }
  
}

struct X
{
  void operator&() {}
  
};

struct Y
{
  operator char&() const {
    static char ch = 0;
    return ch;
  }
  
};

struct Z
{
  template< class T >
  operator T&() const {
    static char ch = 0;
    return *reinterpret_cast<T*>(&ch);
  }
  
};

#include <iostream>
#include <memory>

int main()
{
  X x;
  std::cout << pezzi::addressof(x) << std::endl;
  
  Y y;
  std::cout << &y << std::endl;
  std::cout << std::addressof(y) << std::endl;
  std::cout << pezzi::addressof(y) << std::endl;
  
  Y z;
  std::cout << &z << std::endl;
  std::cout << std::addressof(z) << std::endl;
  std::cout << pezzi::addressof(z) << std::endl;
}
