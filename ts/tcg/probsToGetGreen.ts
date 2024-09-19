import { entriesToObject } from '../utils/entriesToObject';
import { range, inclusiveRange, replicate } from '../utils/generators';
import { sample } from '../utils/random';

const _cardTypes = ['spell', 'multiLand', 'valueLand', 'basicLand', 'vergeLand'] as const;

type CardType = (typeof _cardTypes)[number];

const inspectResults = ['ok', 'bad', 'screw', 'flood'] as const;
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
    const results: Record<InspectResult, number> = entriesToObject(inspectResults.map((result) => [result, 0]));
    const trials = 10000000;
    for (const _ of range(trials)) {
      const result = inspectHand(sample(7, deck));
      results[result] += 1;
    }
    console.log(`results (${trials} trials):`);
    inspectResults.forEach((result) => {
      console.log(`${result}:`, results[result] / trials);
    });
    console.log('');
  }
};
main();
