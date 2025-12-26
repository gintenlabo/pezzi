import { yield_all, wrapTask } from './microthread';

const X = wrapTask(function* (a: string) {
  console.log(`(1): X(${JSON.stringify(a)})`);
  yield;
  console.log(`(3): X(${JSON.stringify(a)})`);
  throw new Error('Error!');
});

const main = wrapTask(function* () {
  yield;
  X('arg1');
  X('arg2');
  console.log('(2)');
  for (let i = 0; i < 10; i++) {
    yield;
  }
  yield;
  console.log('(4)');
  yield;
});

main();

while (yield_all());
