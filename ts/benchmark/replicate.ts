import * as Benchmark from 'benchmark';

const replicateByFrom = <const T>(n: number, x: T): T[] => Array.from({ length: n }, () => x);

const replicateByFill = <const T>(n: number, x: T): T[] => Array<T>(n).fill(x);

const benchReplicate = (replicate: (n: number, x: number) => number[]) => {
  const arr = replicate(10000000, 5);
  for (let i = 0; i < 5; i += 1) {
    void arr.reduce((x, y) => x + y, 0);
  }
};

const suite = new Benchmark.Suite();

suite
  .add('Array.from version', () => {
    benchReplicate(replicateByFrom);
  })
  .add('fill version', () => {
    benchReplicate(replicateByFill);
  })
  .on('cycle', (event: Benchmark.Event) => {
    console.log(String(event.target));
  })
  .on('complete', function (this: Benchmark.Suite) {
    const fastests = this.filter('fastest').map('name') as string[];
    console.log(`Fastest is ${JSON.stringify(fastests)}`);
  })
  .run({ async: true });
