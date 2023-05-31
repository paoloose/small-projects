import { PuppeteerLaunchOptions, ResourceType, Viewport } from 'puppeteer';

interface HeadlessBrowserConfig {
  viewport: Viewport,
  launch: PuppeteerLaunchOptions,
  blockedResourceTypes: ResourceType[],
}
