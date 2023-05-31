import puppeteer, { Browser } from 'puppeteer';
import { browserConfig } from './config';
import { HeadlessBrowserConfig } from '../types';

class HeadlessBrowser {
  browser: Promise<Browser>
  config: HeadlessBrowserConfig

  constructor(config: HeadlessBrowserConfig) {
    this.browser = puppeteer.launch(config.launch)
    this.config = config
  }

  async createPage() {
    const browser = await this.browser;
    const page = await browser.newPage();
    await page.setViewport(browserConfig.viewport);
    await page.setRequestInterception(true);

    page.on('request', request => {
      if (request.resourceType() === 'image') request.abort();
      else request.continue();
    });

    return page;
  }
}

export const browser = new HeadlessBrowser(browserConfig);
