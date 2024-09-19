type UnionToIntersection<U> = (U extends unknown ? (k: U) => void : never) extends (k: infer I) => void ? I : never;
type FromEntries<T extends Iterable<readonly [PropertyKey, unknown]>> = UnionToIntersection<
  T extends Iterable<infer P>
    ? P extends readonly [infer Key extends PropertyKey, infer Value]
      ? {
          [key in Key]: Value;
        }
      : never
    : never
>;

export const entriesToObject = <T extends Iterable<readonly [PropertyKey, unknown]>>(entries: T): FromEntries<T> =>
  Object.fromEntries(entries) as FromEntries<T>;
