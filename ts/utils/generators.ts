export function* range(n: number): Generator<number> {
  for (let i = 0; i < n; i += 1) {
    yield i;
  }
}

export function* inclusiveRange(a: number, b: number): Generator<number> {
  for (let i = a; i <= b; i += 1) {
    yield i;
  }
}

export function* replicate<const T>(n: number, x: T): Generator<T> {
  for (const _ of range(n)) {
    yield x;
  }
}
