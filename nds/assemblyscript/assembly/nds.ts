export declare function swiWaitForVBlank(): void;

const encode = (str: string): ArrayBuffer => String.UTF8.encode(str, true);

declare function _print(ptr: ArrayBuffer): void;
export function print(str: string): void {
  _print(encode(str));
}

export declare function consoleDemoInit(): void;
export declare function consoleClear(): void;

export declare function oamUpdate(): void;
export declare function keysHeld(): u32;
export declare function scanKeys(): void;

export enum KEYS {
  SELECT = 1 << 2,
  START = 1 << 3,
  RIGHT = 1 << 4,
  LEFT = 1 << 5,
  UP = 1 << 6,
  DOWN = 1 << 7,
}

export declare function rand(): u8;

declare function _fillRedRect(x: u8, y: u8, w: u8, h: u8): void;
export function fillRedRect(x: u8, y: u8, w: u8, h: u8): void {
  _fillRedRect(x, y, w, h);
}

export declare function syncFrame(): void;

declare function _btn(btn: u8): bool;
export function btn(btn: u8): bool {
  return _btn(btn);
}

declare function _btnp(btn: u8): bool;
export function btnp(btn: u8): bool {
  return _btnp(btn);
}

