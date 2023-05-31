import { NextFunction, Request, Response } from 'express';

export const cors = (req: Request, res: Response, next: NextFunction) => {
  res.appendHeader('Access-Control-Allow-Origin', '*');
  next();
}
