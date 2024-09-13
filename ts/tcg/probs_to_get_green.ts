function* range(n: number): Generator<number> {
  for (let i = 0; i < n; i += 1) {
    yield i;
  }
}
function* inclusiveRange(a: number, b: number): Generator<number> {
  for (let i = a; i <= b; i += 1) {
    yield i;
  }
}

const getRandomInt = (min: number, max: number): number => {
  const minCeiled = Math.ceil(min);
  const maxFloored = Math.floor(max);
  return Math.floor(Math.random() * (maxFloored - minCeiled) + minCeiled);
};

const sample = <T>(n: number, arr: readonly T[]): T[] => {
  const result = [...arr];
  const len = result.length;
  for (const i of range(n)) {
    if (i >= len) {
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

const _cardTypes = ['spell', 'multiLand', 'valueLand', 'basicLand', 'vergeLand'] as const;

type CardType = (typeof _cardTypes)[number];

const inspectResults = ['ok', 'bad', 'screw', 'flood'];
type InspectResult = (typeof inspectResults)[number];

const inspectHand = (hand: CardType[]): InspectResult => {
  let multi = 0;
  let value = 0;
  let basic = 0;
  let verge = 0;
  hand.forEach((card) => {
    switch (card) {
      case 'multiLand':
        multi += 1;
        break;
      case 'valueLand':
        value += 1;
        break;
      case 'basicLand':
        basic += 1;
        break;
      case 'vergeLand':
        verge += 1;
        break;
      default:
        break;
    }
  });
  const lands = multi + value + basic + verge;
  if (lands <= 1) {
    return 'screw';
  } else if (lands >= 5) {
    return 'flood';
  } else if (multi >= 1) {
    return 'ok';
  } else if (basic >= 1 && verge >= 1) {
    return 'ok';
  }
  return 'bad';
};

const cardsInDeck = 60;
const spells = 39;
const multiLands = 8;
const valueLands = 2;

function* replicate<const T>(n: number, x: T): Generator<T> {
  for (const _ of range(n)) {
    yield x;
  }
}

const main = () => {
  for (const vergeLands of inclusiveRange(0, 4)) {
    const basicLands = cardsInDeck - spells - multiLands - valueLands - vergeLands;
    console.log('basic lands:', basicLands);
    console.log('verge lands:', vergeLands);
    const deck: CardType[] = [
      ...replicate(spells, 'spell'),
      ...replicate(multiLands, 'multiLand'),
      ...replicate(valueLands, 'valueLand'),
      ...replicate(basicLands, 'basicLand'),
      ...replicate(vergeLands, 'vergeLand'),
    ];
    const results: Record<InspectResult, number> = Object.fromEntries(inspectResults.map((result) => [result, 0]));
    const trials = 10000000;
    for (const _ of range(trials)) {
      const result = inspectHand(sample(7, deck));
      // eslint-disable-next-line @typescript-eslint/no-non-null-assertion
      results[result]! += 1;
    }
    console.log(`results (${trials} trials):`);
    inspectResults.forEach((result) => {
      // eslint-disable-next-line @typescript-eslint/no-non-null-assertion
      console.log(`${result}:`, results[result]! / trials);
    });
    console.log('');
  }
};
main();
