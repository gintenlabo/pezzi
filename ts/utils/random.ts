import { range } from './generators';

export const getRandomInt = (min: number, max: number): number => {
  const minCeiled = Math.ceil(min);
  const maxFloored = Math.floor(max);
  return Math.floor(Math.random() * (maxFloored - minCeiled) + minCeiled);
};

export const sample = <T>(n: number, arr: Iterable<T>): T[] => {
  const result = [...arr];
  const len = result.length;
  for (const i of range(n)) {
    if (i >= len - 1) {
      break;
    }
    const j = getRandomInt(i, len);
    // eslint-disable-next-line @typescript-eslint/no-non-null-assertion
    [result[i], result[j]] = [result[j]!, result[i]!];
  }
  if (result.length > n) {
    result.length = n;
  }
  return result;
};
