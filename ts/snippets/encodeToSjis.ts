import yargs from 'yargs';
import fs from 'fs/promises';
import iconv from 'iconv-lite';

interface Options {
  inputFile: string;
  outputFile: string;
}
const getOptions = (): Options => {
  const args = yargs
    .command('*', 'Convert text file to Shift_JIS encoding')
    .options({
      'input-file': {
        type: 'string',
        alias: 'i',
        demandOption: true,
        default: '/dev/stdin',
        describe: 'Input file path',
      },
      'output-file': {
        type: 'string',
        alias: 'o',
        demandOption: true,
        default: './out.txt',
        describe: 'Output file path',
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
    outputFile: args['output-file'],
  };
};

const main = async () => {
  const { inputFile, outputFile } = getOptions();
  const input = await fs.readFile(inputFile, { encoding: 'utf-8' });
  const encodedText = iconv.encode(input, 'Shift_JIS');
  await fs.writeFile(outputFile, encodedText, { mode: 0o644 });
};
void main();
