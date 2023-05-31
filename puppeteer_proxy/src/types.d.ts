import { PuppeteerLaunchOptions, Viewport } from 'puppeteer';

interface HeadlessBrowserConfig {
  viewport: Viewport,
  launch: PuppeteerLaunchOptions,
}
