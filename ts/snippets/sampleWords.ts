import fs from 'fs/promises';

import { sample } from '../utils/random';

const main = async () => {
  const contents = await fs.readFile('/usr/share/dict/words', { encoding: 'utf-8' });
  const allWords = contents.split('\n');
  const chosenWords = sample(4, allWords);
  chosenWords.forEach((word) => {
    console.log(word);
  });
};
void main();
