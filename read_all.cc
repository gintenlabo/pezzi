// ファイルの中身をすべて読み込む関数

#include <iostream>
#include <string>
#include <cassert>
#include <memory>
#include <sstream>

// 万能版． ちょっと低速．
std::string read_all_( std::istream& is ) {
  // 1kiB ずつ愚直に読み込む
  std::size_t const bufsize = 1024;
  char buf[bufsize];
  
  std::string contents;
  
  do {
    is.read( buf, bufsize );
    contents.append( buf, is.gcount() );
  }
  while( is.good() );
  
  return contents;
}

// seek が可能な場合に最適化する
std::string read_all_without_strbuf( std::istream& is ) {
  // 末尾までのサイズを測定
  auto const pos = is.tellg();
  
  // サイズ測定が失敗した場合
  if( pos == -1 ) {
    return ::read_all_(is);
  }
  
  // サイズ測定に成功した場合
  is.seekg( 0, std::ios::end );
  std::size_t const n = is.tellg() - pos;
  is.seekg( pos, std::ios::beg );
  
  if( n == 0 ) {
    return {};
  }
  
  std::string contents( n, '\0' );
  
  is.read( &contents[0], n ); // data が使いたい．
  
  return contents;
}

std::string read_all_with_stringstream( std::istream& is ){
  std::ostringstream oss;
  is >> oss.rdbuf();
  return oss.str();
}

std::string read_all_with_stringbuf( std::istream& is ){
  std::stringbuf strbuf( std::ios::out );
  is >> &strbuf;
  return strbuf.str();
}

#include <boost/progress.hpp>
#include <fstream>

int main( int argc, char* argv[] )
{
  if( argc != 2 ) {
    std::cerr << "usage: " << argv[0] << " filename\n";
    return 1;
  }
  
  char const* filename = argv[1];
  
  // 読み捨て（キャッシュ対策）
  {
    std::ifstream fs( filename );
    std::string line;
    std::size_t n = 0;
    
    while( std::getline( fs, line ) ) {
      n += line.size() + 1;
    }
    
    std::cout << filename << ":  " << n << " charactors.\n\n";
    
  }
  
  {
    std::cout << "stringbuf...\n";
    
    std::ifstream fs( filename );
    std::string s;
    
    boost::progress_timer t;
    s = read_all_with_stringbuf(fs);
    std::cout << "read " << s.size() << " charactors.\n";
  }
  {
    std::cout << "without strbuf...\n";
    
    std::ifstream fs( filename );
    std::string s;
    
    boost::progress_timer t;
    s = read_all_without_strbuf(fs);
    std::cout << "read " << s.size() << " charactors.\n";
  }
  
  {
    std::cout << "stringstream...\n";
    
    std::ifstream fs( filename );
    std::string s;
    
    boost::progress_timer t;
    s = read_all_with_stringstream(fs);
    std::cout << "read " << s.size() << " charactors.\n";
  }
  
}
