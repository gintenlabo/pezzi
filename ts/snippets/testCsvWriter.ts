import { createObjectCsvStringifier } from 'csv-writer';

const main = () => {
  const csvStringifier = createObjectCsvStringifier({
    header: [
      { id: 'a', title: 'AAA' },
      { id: 'blank', title: 'blank 1' },
      { id: 'b', title: 'BBB' },
      { id: 'blank', title: 'blank 2' },
    ],
  });
  console.log(
    `${csvStringifier.getHeaderString()}${csvStringifier.stringifyRecords([
      { a: 'a1', b: 42 },
      { a: 'value with ","', b: 13 },
    ])}`,
  );
};
main();
