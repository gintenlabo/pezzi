-- ベンチマーク用ユーティリティ
-- 特にモジュール化とかは行わない。必要なら呼び出した側で module すること。

-- 関数 f(...) を n 回実行するのにかかった時間を測定する
do
  local clock = os.clock
  
  function time( n, f, ... )
    local t = clock()
    
    for i = 1, n do
      local x = f(...)
    end
    
    return clock() - t
  end
end

-- 関数 f(...) を一秒間に何回実行出来るか測定する
do
  local time = time
  local collectgarbage = collectgarbage
  
  function benchmark( f, ... )
    -- 予め GC を呼ぶ
    collectgarbage("collect")
    
    local step = 1000  -- 最低 1000 回は実行される
    
    local n = 0
    local t = 0
    while true do
      t = t + time( step, f, ... )
      n = n + step
      
      if t >= 1 then break end
      
      -- 次の実行ではステップを倍にする
      step = step * 2
    end
    
    return n / t
  end
end

-- 数値を丸める
do
  local floor = math.floor
  function round(x)
    return floor( x + 0.5 )
  end
end

-- ベンチマークして表示する
do
  local print = print
  local round = round
  
  function show_benchmark( name, f, ... )
    print( name, round( benchmark( f, ... ) ) )
  end
end
