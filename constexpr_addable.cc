/*
なにジョジョ？
operator+= から operator+ を生成すると constexpr にできない？

ジョジョ　それは
operator+= を使うからだよ

　　　　　＿_.. -―─ ､__
　　　 ／｀　　　　　　 三ﾐー ﾍ､_
　　ゝ'　;; ,, , ,,　　　　 ミﾐ　 , il　ﾞＺ,
　 _〉,..　　　 ////, ,彡ffｯｨ彡从j彡
　 〉,ｨiiif ,　,, 'ノ川jﾉ川;　:.｀ﾌ公)了
　＼.:.:.:i=珍／二''=く､ !ノ一ヾﾞ;.;.;）
　 く:.:.:.:lﾑjｲ 　rfﾓﾃ〉ﾞ} ijｨtｹ 1ｲ'´
　　 〕:.:.|,Y!:!､ 　 ﾆ '､ ;　|｀ﾆ ｲj'　　逆に考えるんだ
　　 {:.:.:j {: :}　｀　　 ､_{__}　　/ﾉ
　 　 〉ｲ　､ﾞ!　　　,ィ__三ｰ､ j′　　「 operator+ から operator+= を生成すればいいさ」
　　,{　＼ ﾐ ＼　 ゝ' ｪｪ' ｀' /
-‐'　＼　＼ ヽ＼　　彡　ｲ-､　　　　と考えるんだ
　　　　 ＼　＼.ヽゝ‐‐‐升 ﾄ､ ヽ､__
　　　　　　＼　 ヽ- ､./／ j!:.}　　　 ｀ ｰ ､
　　　　　　 ヽ＼　厶_r__ﾊ/!:.{
　　　　　　　　　 ´　/ ! ヽ 
*/

#include <utility>

namespace pezzi
{
  template< class T >
  struct addable
  {
    friend T& operator+=( T& lhs, T const& rhs ) {
      lhs = std::move(lhs) + rhs;
      return lhs;
    }
    
    friend T& operator+=( T& lhs, T && rhs ) {
      lhs = std::move(lhs) + std::move(rhs);
      return lhs;
    }
    
  };
  
}

namespace ns
{
  struct hoge
    : private pezzi::addable<hoge>
  {
    int i;
    
    explicit constexpr hoge( int i_ )
      : i( i_ ) {}
    
    friend constexpr hoge operator+( hoge const& lhs, hoge const& rhs )
    {
      return hoge( lhs.i + rhs.i );
    }
    
  };
  
}

#include <iostream>

int main()
{
  using ns::hoge;
  
  constexpr hoge h1(1);
  constexpr hoge h2(2);
  constexpr hoge h3 = h2 + h1;
  
  hoge h4(4);
  h4 += h3;
  
  std::cout << h4.i << std::endl;
  
}
