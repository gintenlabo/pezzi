import { sample } from '../utils/random';
import { range } from '../utils/generators';

const test = (n: number, m: number) => {
  console.log(`sample(${n}, range(${m})):`);
  for (const _ of range(10)) {
    console.log(sample(n, range(m)));
  }
  console.log('');
};

// 不足
test(10, 5);

// ジャスト
test(5, 5);

// 少数サンプリング
test(5, 100);
