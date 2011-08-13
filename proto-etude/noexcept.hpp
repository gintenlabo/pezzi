#ifndef PROTO_ETUDE_INCLUDED_NOEXCEPT_HPP_
#define PROTO_ETUDE_INCLUDED_NOEXCEPT_HPP_

// ETUDE_NOEXCEPT(expr) マクロ
//  noexcept にコンパイラが対応しているなら noexcept(expr) を，
//  対応していないなら false を返す

// ETUDE_NOEXCEPT_IF(expr) マクロ
//  result f( args ) ETUDE_NOEXCEPT_IF(expr) のように使う
//  noexcept にコンパイラが対応しているなら noexcept(expr) に展開される
//  そうでなければ空文字列に展開される

#if defined(ETUDE_USING_NO_NOEXCEPT) || \
  ( __GNUC__ < 4 || ( __GNUC__ == 4 && __GNUC_MINOR__ < 6 ) )

  #define ETUDE_NOEXCEPT(expr)    ( sizeof( (void)(expr), 0 ) == 0 )
  #define ETUDE_NOEXCEPT_IF(expr)

#else

  #if __GNUC__ == 4 && __GNUC_MINOR__ == 6 && __GNUC_PATCHLEVEL__ == 0
    #define ETUDE_NOEXCEPT(expr)  ( sizeof( (void)(expr), 0 ) > 0 && noexcept( expr ) )
  #else
    #define ETUDE_NOEXCEPT(expr)  noexcept(expr)
  #endif
  
  #define ETUDE_NOEXCEPT_IF(expr) noexcept(expr)

#endif

#endif  // #ifndef PROTO_ETUDE_INCLUDED_NOEXCEPT_HPP_
