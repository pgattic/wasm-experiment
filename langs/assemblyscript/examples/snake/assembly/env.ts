const encode = (str: string): ArrayBuffer => String.UTF8.encode(str, true);

declare function _rand(): u32;
export function rand(): u32 {
  return _rand();
}

declare function _clearScreen(c: u8): void;
export function clearScreen(c: u8): void {
  return _clearScreen(c);
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
  _rectFill(x, y, w, h, c);
}

declare function _sprite(x: u8, y: u8, id: u8): void;
export function sprite(x: u8, y: u8, id: u8): void {
  _sprite(x, y, id);
}

declare function _btn(btn: u8): bool;
export function btn(btn: Button): bool {
  return _btn(btn as u8);
}

declare function _btnP(btn: u8): bool;
export function btnP(btn: Button): bool {
  return _btnP(btn as u8);
}

declare function _printLnDbg(ptr: ArrayBuffer): void;
export function print(str: string): void {
 _printLnDbg(encode(str));
}

export enum Button {
    Left = 0,
    Right = 1,
    Up = 2,
    Down = 3,
    A = 4,
    B = 5,
    X = 6,
    Y = 7,
    L = 8,
    R = 9,
    Start = 10,
    Select = 11,
}

