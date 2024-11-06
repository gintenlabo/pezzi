import yargs from 'yargs';
import DateFns from 'date-fns';
import { tz, TZDate } from '@date-fns/tz';

const tzUTC = tz('UTC');

const truncateDateTimeToDayInUTC = (date: Date): Date => {
  return DateFns.startOfDay(date, { in: tzUTC });
};
const roundDateTimeToDayInUTC = (date: Date): Date => {
  return truncateDateTimeToDayInUTC(DateFns.addHours(date, 12));
};
const getStartOfDayFromDayInUTC = (date: Date, timezone: string): TZDate => {
  return TZDate.tz(timezone, date.getUTCFullYear(), date.getUTCMonth(), date.getUTCDate());
};
const isBetweenInDay = (date: Date, startDay: Date, endDay: Date, timezone: string): boolean => {
  const startDayInUTC = roundDateTimeToDayInUTC(startDay);
  const start = getStartOfDayFromDayInUTC(startDayInUTC, timezone);
  const endDayInUTC = roundDateTimeToDayInUTC(endDay);
  const end = DateFns.endOfDay(getStartOfDayFromDayInUTC(endDayInUTC, timezone), { in: tz(timezone) });
  return DateFns.isWithinInterval(date, { start, end });
};

interface Options {
  startDay: string;
  endDay: string;
  testedTime: string | null;
  timezone: string;
}
const getOptions = (): Options => {
  const args = yargs
    .command('*', 'Test datetime is between start day and end day')
    .options({
      'start-day': {
        type: 'string',
        alias: 's',
        demandOption: true,
        describe: 'start day',
      },
      'end-day': {
        type: 'string',
        alias: 'e',
        demandOption: true,
        describe: 'end day',
      },
      'tested-time': {
        type: 'string',
        alias: 't',
        demandOption: false,
        describe: 'target datetime (default: now)',
      },
      timezone: {
        type: 'string',
        alias: 'z',
        demandOption: true,
        default: 'Asia/Tokyo',
        describe: 'target timezone',
      },
    })
    .alias({
      h: 'help',
    })
    .version(false)
    .strict()
    .parseSync();

  return {
    startDay: args['start-day'],
    endDay: args['end-day'],
    testedTime: args['tested-time'] ?? null,
    timezone: args['timezone'],
  };
};

const main = () => {
  const { startDay, endDay, testedTime, timezone } = getOptions();
  console.log(
    isBetweenInDay(
      testedTime == null ? new Date() : new Date(testedTime),
      new Date(startDay),
      new Date(endDay),
      timezone,
    ),
  );
};
main();
