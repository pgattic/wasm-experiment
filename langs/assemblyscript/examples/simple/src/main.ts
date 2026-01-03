import * as env from "./env";

let playerX: i32;
let playerY: i32;

export function setup(): void {
  playerX = 8;
  playerY = 8;
}

export function update(): void {

  // Handle input
  if (env.btn(env.Button.Left))  { playerX -= 4; }
  if (env.btn(env.Button.Right)) { playerX += 4; }
  if (env.btn(env.Button.Up))    { playerY -= 4; }
  if (env.btn(env.Button.Down))  { playerY += 4; }

  // Render Screen
  env.clearScreen(1);
  env.rectFill(playerX, playerY, 8, 8, 11);
}

