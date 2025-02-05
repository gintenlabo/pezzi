import yargs from 'yargs';
import fs from 'fs/promises';

import { range } from '../utils/generators';
import { getRandomInt } from '../utils/random';

interface Options {
  inputFile: string;
  words: number;
}

const getOptions = (): Options => {
  const args = yargs
    .command('*', 'Convert text file to Shift_JIS encoding')
    .options({
      'input-file': {
        type: 'string',
        alias: 'i',
        demandOption: true,
        default: '/usr/share/dict/words',
        describe: 'Input file path',
      },
      words: {
        type: 'number',
        alias: 'n',
        demandOption: true,
        default: 4,
        describe: 'word count to sample (all line if 0 is given)',
      },
    })
    .alias({
      h: 'help',
    })
    .version(false)
    .strict()
    .parseSync();

  return {
    inputFile: args['input-file'],
    words: args['words'],
  };
};

const main = async () => {
  const { inputFile, words } = getOptions();
  const contents = await fs.readFile(inputFile, { encoding: 'utf-8' });
  const allWords = contents.split('\n').filter((str) => str !== '');

  const len = allWords.length;
  const n = words <= 0 ? len : words;
  for (const i of range(n)) {
    if (i >= len) {
      break;
    }
    const j = getRandomInt(i, len);
    // eslint-disable-next-line @typescript-eslint/no-non-null-assertion
    const sampled = allWords[j]!;
    // eslint-disable-next-line @typescript-eslint/no-non-null-assertion
    allWords[j] = allWords[i]!;
    allWords[i] = sampled;
    console.log(sampled);
  }
};
void main();
