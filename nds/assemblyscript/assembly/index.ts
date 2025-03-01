import * as nds from "./nds";

export function start(): void {

  nds.consoleDemoInit(); // Initialize console

  nds.print("\n Hello from WASM!\n");

  while (true) {
    // Wait for the screen refresh
    nds.swiWaitForVBlank();
  }

}

