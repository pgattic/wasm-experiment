const encode = (str: string): ArrayBuffer => String.UTF8.encode(str, true);

//declare function _print(ptr: ArrayBuffer): void;
//export function print(str: string): void {
//  _print(encode(str));
//}

declare function _rand(): u32;
export function rand(): u32 {
  return _rand();
}

declare function _pSet(x: u8, y: u8, c: u8): void;
export function pSet(x: u8, y: u8, c: u8): void {
  _pSet(x, y, c);
}

declare function _rect(x: u8, y: u8, w: u8, h: u8, c: u8): void;
export function rect(x: u8, y: u8, w: u8, h: u8, c: u8): void {
  _rect(x, y, w, h, c);
}

declare function _rectFill(x: u8, y: u8, w: u8, h: u8, c: u8): void;
export function rectFill(x: u8, y: u8, w: u8, h: u8, c: u8): void {
  _fillRect(x, y, w, h, c);
}

declare function _syncFrame(): void;
export function syncFrame(): void {
  _syncFrame();
}

declare function _btn(btn: u8): bool;
export function btn(btn: u8): bool {
  return _btn(btn);
}

declare function _btnP(btn: u8): bool;
export function btnP(btn: u8): bool {
  return _btnP(btn);
}

