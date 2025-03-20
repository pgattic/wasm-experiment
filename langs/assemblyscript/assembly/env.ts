const encode = (str: string): ArrayBuffer => String.UTF8.encode(str, true);

declare function _print(ptr: ArrayBuffer): void;
export function print(str: string): void {
  _print(encode(str));
}

declare function _rand(): u8;
export function rand(): u8 {
  return _rand();
}

declare function _fillRedRect(x: u8, y: u8, w: u8, h: u8): void;
export function fillRedRect(x: u8, y: u8, w: u8, h: u8): void {
  _fillRedRect(x, y, w, h);
}

declare function _syncFrame(): void;
export function syncFrame(): void {
  _syncFrame();
}

declare function _btn(btn: u8): bool;
export function btn(btn: u8): bool {
  return _btn(btn);
}

declare function _btnp(btn: u8): bool;
export function btnp(btn: u8): bool {
  return _btnp(btn);
}

