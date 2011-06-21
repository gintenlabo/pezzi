template<class T>
void f() {
  []( bool b ){ return b; };
}

int main()
{
  f<int>();
}
