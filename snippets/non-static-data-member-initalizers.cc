struct X{
  int i = 42;
};

#include <cassert>

int main()
{
  X x;
  assert( x.i == 42 );
}
