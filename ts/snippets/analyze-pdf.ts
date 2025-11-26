import * as fs from 'fs/promises';
import yargs from 'yargs';
import * as pdfjsLib from 'pdfjs-dist/legacy/build/pdf.mjs';

interface Options {
  inputFile: string;
  outputFile: string;
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

async function main() {
  const { inputFile, outputFile } = getOptions();
  const pdfData = new Uint8Array(await fs.readFile(inputFile));
  const pdf = await pdfjsLib.getDocument({ data: pdfData }).promise;
  const maxPages = pdf.numPages;

  const pages = await Promise.all(
    Array.from({ length: maxPages }, (_, i) => i + 1).map(async (pageNumber) => {
      const page = await pdf.getPage(pageNumber);
      return page.getTextContent();
    }),
  );

  await fs.writeFile(outputFile, JSON.stringify(pages, null, 2));
}

main().catch((err: unknown) => {
  console.error(err);
});
