import { dnh_yield, wrapTask } from './microthread';

const X = wrapTask(async (a: string) => {
  console.log(`(1): X(${JSON.stringify(a)})`);
  await dnh_yield();
  console.log(`(3): X(${JSON.stringify(a)})`);
  throw new Error('Error!');
});

async function main() {
  await dnh_yield();
  X('arg1');
  X('arg2');
  console.log('(2)');
  await dnh_yield();
  console.log('(4)');
}

main().catch((err: unknown) => {
  console.error(err);
});
