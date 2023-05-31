import express from 'express';
import dotenv from 'dotenv';
import { browser } from './browser';
import { cors } from './cors';

dotenv.config();
const app = express();
const PORT = Number(process.env.PORT) | 8080;

app.use(cors);

app.get('/browse', async (req, res) => {
  const { url } = req.query;

  if (typeof url !== 'string') {
    res.status(400).send({
      code: 400,
      error: 'No URL provided'
    });
    return;
  }

  const page = await browser.createPage();

  await page.goto(url);

  await page.evaluate((foreignUrl: string) => {
    const baseTag = document.querySelector('base') || document.createElement('base');
    baseTag.href = foreignUrl;
    document.head.prepend(baseTag);
  }, url)

  const content = await page.content();
  res.type('text/plain').send(content);

  await page.close();
});

app.listen(PORT, () => {
  console.log(`[server]: running at http://localhost:${PORT}`);
});
