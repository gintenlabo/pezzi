import { range, inclusiveRange, replicate } from './generators';

const GeneratorFunction = function* () {}.constructor;

describe('range', () => {
  it('should be a GeneratorFunction', () => {
    expect(range).toBeInstanceOf(GeneratorFunction);
  });
  it('should return values [0, n)', () => {
    expect([...range(5)]).toEqual([0, 1, 2, 3, 4]);
  });
  it('should return no values if n <= 0', () => {
    expect([...range(-1)]).toEqual([]);
  });
});

describe('inclusiveRange', () => {
  it('should be a GeneratorFunction', () => {
    expect(inclusiveRange).toBeInstanceOf(GeneratorFunction);
  });
  it('should return values [a, b]', () => {
    expect([...inclusiveRange(1, 3)]).toEqual([1, 2, 3]);
  });
  it('should return single value if a === b', () => {
    expect([...inclusiveRange(2, 2)]).toEqual([2]);
  });
  it('should return no values when a > b', () => {
    expect([...inclusiveRange(3, 2)]).toEqual([]);
  });
});

describe('replicate', () => {
  it('should be a GeneratorFunction', () => {
    expect(replicate).toBeInstanceOf(GeneratorFunction);
  });
  it('should return n copies of x', () => {
    expect([...replicate(5, 'a')]).toEqual(['a', 'a', 'a', 'a', 'a']);
  });
  it('should return no values if n <= 0', () => {
    expect([...replicate(-1, 'a')]).toEqual([]);
  });
});
