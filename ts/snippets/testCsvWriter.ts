import { createObjectCsvStringifier } from 'csv-writer';

const main = () => {
  const csvStringifier = createObjectCsvStringifier({
    header: [
      { id: 'a', title: 'AAA' },
      { id: 'unknownKey', title: 'Unknown Key' },
      { id: 'b', title: 'BBB' },
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
