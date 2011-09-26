-- 不動点コンビネータの実装による速度の違いを計測

-- 最初の版（どう見ても効率が悪い）
local function fix1(f)
  return function(...)
    return f( fix1(f), ... )
  end
end

-- 改善版
local function fix2(f)
  local function fixed(...)
    return f( fixed, ... )
  end
  return fixed
end

-- 別バージョン（再帰なし）
local function fix3( curried )
  local fixed;
  fixed = curried(
    function(...)
      return fixed(...)
    end
  )
  return fixed
end

-- ベンチマーク

-- 関数 f(...) を実行するのにかかった時間を測定
do
  local clock = os.clock
  local unpack = unpack
  
  function time( f, ... )
    local t = clock()
    local results = { f(...) }
    return clock() - t, unpack(results)
  end
end

-- 竹内関数を使う
local function tarai_( tarai, x, y, z )
  if x <= y then
    return y
  else
    return tarai( tarai( x-1, y, z ), tarai( y-1, z, x ), tarai( z-1, x, y ) )
  end
end

-- 普通に再帰で書いた場合
local function tarai0( x, y, z )
  if x <= y then
    return y
  else
    return tarai0( tarai0( x-1, y, z ), tarai0( y-1, z, x ), tarai0( z-1, x, y ) )
  end
end

-- 低速版 tarai
local tarai1 = fix3( function( tarai ) return function( x, y, z ) return tarai_( tarai, x, y, z ) end end )
local tarai2 = fix2( tarai_ )


-- ベンチマーク関数
local function benchmark( tarai )
  -- 予め GC を呼ぶ
  collectgarbage("collect")
  -- 測定
  local t, result = time( tarai, 12, 6, 0 )
  -- 表示
  print( string.format( "%d, %fs", result, t ) )
end

-- 計測開始
print( "non-lambda version:" )
benchmark( tarai0 )
print( "" )

print( "fix(slow) version:" )
benchmark( tarai1 )
print( "" )

print( "fix(fast) version:" )
benchmark( tarai2 )
print( "" )

