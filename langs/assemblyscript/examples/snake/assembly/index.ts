import * as env from "./env";

enum Direction { Up, Down, Left, Right, }

const screenDimensions: Array<u16> = [256, 192];

const gridSize: u8 = 8;

const gridDimensions: Array<u8> = [(screenDimensions[0] / gridSize as u16) as u8, (screenDimensions[1] / gridSize as u16) as u8];

let paused: bool = false;

let playerBody: Array<Array<u8>> = [[0, 0]];
let playerDirection = Direction.Right;
let playerSize = 5;

let frameCount: u8 = 0;

let foodPos: Array<u8> = [(env.rand() % gridDimensions[0] as u32) as u8, (env.rand() % gridDimensions[1] as u32) as u8];

export function setup(): void {}

export function update(): void {

  if (env.btnP(env.Button.Left) && playerDirection != Direction.Right) { playerDirection = Direction.Left; }
  if (env.btnP(env.Button.Right) && playerDirection != Direction.Left) { playerDirection = Direction.Right; }
  if (env.btnP(env.Button.Up) && playerDirection != Direction.Down) { playerDirection = Direction.Up; }
  if (env.btnP(env.Button.Down) && playerDirection != Direction.Up) { playerDirection = Direction.Down; }
  if (env.btnP(env.Button.Start)) { paused = !paused; }

  // Game loop
  if (frameCount >= 6 && !paused) {
    frameCount = 0;
    let playerHead = playerBody[0];

    // Eat food
    if (playerHead[0] == foodPos[0] && playerHead[1] == foodPos[1]) {
      playerSize += 5;
      foodPos = [(env.rand() % gridDimensions[0] as u32) as u8, (env.rand() % gridDimensions[1] as u32) as u8];
    }

    // Advance player
    switch (playerDirection) {
      case Direction.Left:
        playerBody.unshift([playerHead[0] - 1, playerHead[1]]);
        break;
      case Direction.Right:
        playerBody.unshift([playerHead[0] + 1, playerHead[1]]);
        break;
      case Direction.Up:
        playerBody.unshift([playerHead[0], playerHead[1] - 1]);
        break;
      case Direction.Down:
        playerBody.unshift([playerHead[0], playerHead[1] + 1]);
        break;
    }
    // Ensure player is the correct size
    while (playerBody.length > playerSize) {
      playerBody.pop();
    }

    playerHead = playerBody[0];
    // Death/reset cases
    for (let s = 1; s < playerBody.length; s++) {
      const segment = playerBody[s];
      if (playerHead[0] == segment[0] && playerHead[1] == segment[1]) { // Kill
        playerBody = [[0, 0]];
        playerDirection = Direction.Right;
        playerSize = 5;
        foodPos = [(env.rand() % gridDimensions[0] as u32) as u8, (env.rand() % gridDimensions[1] as u32) as u8];
        break;
      }
    }
    if (playerHead[0] >= gridDimensions[0] || playerHead[1] >= gridDimensions[1]) {
      playerBody = [[0, 0]];
      playerDirection = Direction.Right;
      playerSize = 5;
      foodPos = [(env.rand() % gridDimensions[0] as u32) as u8, (env.rand() % gridDimensions[1] as u32) as u8];
    }
  }

  // Rendering
  for (let s = 0; s < playerBody.length; s++) {
    let segment = playerBody[s];
    env.rectFill(segment[0] * gridSize, segment[1] * gridSize, gridSize, gridSize, 11);
  }

  env.sprite(foodPos[0] * gridSize, foodPos[1] * gridSize, 1);

  frameCount++;
}

