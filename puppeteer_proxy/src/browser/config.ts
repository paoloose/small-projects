import { HeadlessBrowserConfig } from "../types";

export const browserConfig: HeadlessBrowserConfig = {
  viewport: {
    width: 800,
    height: 600
  },
  launch: {
    headless: 'new',
    args: [
      '--force-dark-mode',
    ]
  }
};
