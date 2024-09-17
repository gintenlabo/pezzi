import * as Benchmark from 'benchmark';

const replicateByFrom = <const T>(n: number, x: T): T[] => Array.from({ length: n }, () => x);

const replicateByFill = <const T>(n: number, x: T): T[] => Array<T>(n).fill(x);

const benchArray = (arr: readonly number[]) => {
  void arr.reduce((x, y) => x + y, 0);
};
const arrByFrom = replicateByFrom(10000000, 5);
const arrByFill = replicateByFill(10000000, 5);

const suite = new Benchmark.Suite();

suite
  .add('Array.from version', () => {
    benchArray(arrByFrom);
  })
  .add('fill version', () => {
    benchArray(arrByFill);
  })
  .on('cycle', (event: Benchmark.Event) => {
    console.log(String(event.target));
  })
  .on('complete', function (this: Benchmark.Suite) {
    const fastestNames = this.filter('fastest').map('name') as string[];
    console.log(`Fastest is ${JSON.stringify(fastestNames)}`);
  })
  .run({ async: true });
