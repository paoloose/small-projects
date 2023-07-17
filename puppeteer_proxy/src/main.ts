import express from 'express';
import dotenv from 'dotenv';
import { browser } from './browser';
import { cors } from './cors';

dotenv.config();
const app = express();
const PORT = Number(process.env.PORT) | 8080;

app.use(cors);

const cache = new Map<string, string>();

app.get('/browse', async (req, res) => {
  const { url } = req.query;

  if (typeof url !== 'string') {
    res.status(400).send({
      code: 400,
      error: 'No URL provided'
    });
    return;
  }

  if (cache.has(url)) {
    return res.type('text/html').send(cache.get(url));
  }

  const page = await browser.createPage();

  console.log({ url })
  await page.goto(url.replace('http://', 'https://'), {
    waitUntil: 'networkidle2',
    timeout: 69690
  });

  await page.evaluate(() => {
    // create and prepend base element
    const baseTag = document.querySelector('base') || document.createElement('base');
    baseTag.href = window.location.href;
    document.head.prepend(baseTag);

    document.querySelectorAll('*').forEach(el => {
      // Ignore if already removed
      if (!el.parentElement) return;
      // Ignore the following elements
      if (el.tagName === 'BASE') return;
      // if (el.tagName === 'SCRIPT') return;
      if (el.tagName === 'STYLE') return;
      if (el.tagName === 'HEAD') return;
      if (el.tagName === 'LINK' && el.getAttribute('rel')?.includes('stylesheet')) return;

      const { width, height } = el.getBoundingClientRect();
      const computedStyle = window.getComputedStyle(el);

      const hasContent = el.textContent !== null && el.textContent.trim() !== '';
      const noSize = width === 0 || height === 0;
      const noDisplay = computedStyle.display === 'none';
      const noVisible = computedStyle.visibility === 'hidden';
      const noOpacity = computedStyle.opacity === '0';

      if ((noVisible && !hasContent) || (noDisplay && !hasContent) || (noOpacity && !hasContent)) {
        el.remove();
      }
      if (noSize && el.children.length === 0) {
        el.remove();
      }
    });

    // create and append style element
    const stylesheet = `
      *, *::before, *::after {
        user-select: none !important;
        user-drag: none !important;
        -webkit-user-drag: none !important;
      }

      *:hover:not(:has(*:hover)) {
        outline: 1px dashed red !important;
        cursor: default !important;
      }

      *:has(> *:hover) {
        overflow: visible !important;
      }
    `;
    const styleTag = document.createElement('style');
    styleTag.textContent = stylesheet;
    document.head.append(styleTag);
  });

  const content = await page.content();

  // const content = await page.content();
  res.type('text/html').send(content);
  cache.set(url, content);

  await page.close();
});

app.listen(PORT, () => {
  console.log(`[server]: running at http://localhost:${PORT}`);
});
