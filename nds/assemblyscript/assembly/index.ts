import * as nds from "./nds";

export function start(): void {

  let x: u8 = 12;
  let y: u8 = 12;

  while (true) {
    x += 1;
    y += 1;

    nds.fillRedRect(48, 24, 12, 12);
    nds.fillRedRect(x, y, 12, 12);

    // Wait for the screen refresh
    nds.syncFrame();
  }
}

