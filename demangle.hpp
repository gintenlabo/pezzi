#ifndef PEZZI_INCLUDED_DEMANGLE_HPP_
#define PEZZI_INCLUDED_DEMANGLE_HPP_

// demangle : 与えられた type_info をデマングルする． gcc only．
// 参考： 本の虫「gccの名前のデマングル」
//        http://cpplover.blogspot.com/2010/03/gcc.html

#include <typeinfo>
#include <cxxabi.h>
#include <cstdlib>
#include <memory>

namespace pezzi
{
  // malloc されたメモリを解放する関数オブジェクト
  struct heap_deallocate
  {
    void operator()( void* p ) const {
      std::free( p );
    }
  };
  
  struct Demangle
  {
    Demangle( std::type_info const& ti )
      : name_( demangle_(ti) ) {}
  
    char const* name() const {
      return name_.get();
    }
    
    operator char const*() const {
      return name();
    }
  
   private:
    typedef std::unique_ptr<char, heap_deallocate> name_holder;
    name_holder name_;
    
    static name_holder demangle_( std::type_info const& ti ) {
      int status = 0;
      return name_holder( abi::__cxa_demangle( ti.name(), 0, 0, &status ) );
    }
  
  };

} // namespace pezzi

#endif  // #ifndef PEZZI_INCLUDED_DEMANGLE_HPP_
