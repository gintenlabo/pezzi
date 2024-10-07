import fs from 'fs/promises';
import iconv from 'iconv-lite';

const main = async () => {
  const text = 'こんにちは\n';
  const encodedText = iconv.encode(text, 'Shift_JIS');
  await fs.writeFile('out.txt', encodedText, { mode: 0o644 });
};
void main();
