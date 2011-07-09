namespace pezzi
{
  template< class T >
  inline T* addressof( T& x )
  {
    return reinterpret_cast<T*>(
      const_cast<char*>(
        &reinterpret_cast<char const volatile &>(x)
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
  int i; char c;
  
  operator char&() {
    return c;
  }
  
  operator const char&() const {
    return c;
  }
  
  operator volatile char&() volatile {
    return c;
  }
  
  operator const volatile char&() const volatile {
    return c;
  }
  
};

#include <iostream>

int main()
{
  X x;
  std::cout << pezzi::addressof(x) << std::endl;
  
  Y y;
  std::cout << &y << std::endl;
  std::cout << static_cast<void*>(&y.c) << std::endl;
  std::cout << pezzi::addressof(y) << std::endl;
}
