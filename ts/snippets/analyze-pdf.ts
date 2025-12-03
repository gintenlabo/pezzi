import * as fs from 'fs/promises';
import yargs from 'yargs';
import * as pdfjsLib from 'pdfjs-dist/legacy/build/pdf.mjs';

const modes = ['operator', 'text'] as const;
type Mode = (typeof modes)[number];

interface Options {
  inputFile: string;
  outputFile: string;
  mode: Mode;
}
const getOptions = (): Options => {
  const args = yargs
    .command('*', 'Analyze pdf file')
    .options({
      'input-file': {
        type: 'string',
        alias: 'i',
        demandOption: true,
        describe: 'Input file path',
      },
      'output-file': {
        type: 'string',
        alias: 'o',
        demandOption: true,
        default: '/dev/stdout',
        describe: 'Output file path',
      },
      mode: {
        choices: modes,
        alias: 'm',
        demandOption: true,
        default: 'operator' as const,
        describe: 'What to output',
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
    mode: args['mode'],
  };
};

type UnwrapTupleArray<T extends readonly (readonly unknown[])[]> = {
  [Index in keyof T]: T[Index][number];
};

function zip<Args extends readonly (readonly unknown[])[]>(...args: Args): UnwrapTupleArray<Args>[] {
  return Array.from(
    { length: Math.min(...args.map((arg) => arg.length)) },
    (_, idx) => args.map((arg) => arg[idx]) as UnwrapTupleArray<Args>,
  );
}

async function main() {
  const { inputFile, outputFile, mode } = getOptions();
  const pdfData = new Uint8Array(await fs.readFile(inputFile));
  const pdf = await pdfjsLib.getDocument({ data: pdfData }).promise;
  const maxPages = pdf.numPages;

  const pages = await (async () => {
    switch (mode) {
      case 'operator': {
        const operatorNumberToNameMap = new Map<number, string>(
          Object.entries(pdfjsLib.OPS)
            .map(([key, num]): [number, string] | null => {
              if (typeof num !== 'number') {
                return null;
              }
              return [num, key];
            })
            .filter((elem) => elem != null),
        );
        return Promise.all(
          Array.from({ length: maxPages }, (_, i) => i + 1).map(async (pageNumber) => {
            const page = await pdf.getPage(pageNumber);

            const { fnArray, argsArray } = await page.getOperatorList();
            return zip(
              fnArray.map((fnNum) => {
                const fn = operatorNumberToNameMap.get(fnNum);
                if (fn == null) {
                  return String(fnNum);
                }
                return fn;
              }),
              argsArray,
            );
          }),
        );
      }
      case 'text':
        return Promise.all(
          Array.from({ length: maxPages }, (_, i) => i + 1).map(async (pageNumber) => {
            const page = await pdf.getPage(pageNumber);
            return {
              viewport: page.getViewport(),
              textContent: await page.getTextContent(),
            };
          }),
        );
      default:
        mode satisfies never;
    }
  })();

  await fs.writeFile(outputFile, JSON.stringify(pages, null, 2));
}

main().catch((err: unknown) => {
  console.error(err);
});
