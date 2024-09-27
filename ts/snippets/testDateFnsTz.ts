import DateFns from 'date-fns';
import { tz } from '@date-fns/tz';

const tzJST = tz('Asia/Tokyo');
const formatDateInJST = (format: string, date: Date) => {
  return DateFns.format(date, format, { in: tzJST });
};

const main = () => {
  console.log(formatDateInJST('yyyy-MM-dd HH:mm:ss X', new Date('2021-01-01T00:00:00Z')));
  console.log(formatDateInJST('yyyyMMdd', new Date()));
};
main();
