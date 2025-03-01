import * as nds from "./nds";

enum Direction {
  Up,
  Down,
  Left,
  Right,
}

export function start(): void {

  const screenDimensions: Array<u16> = [256, 192];

  const gridSize: u8 = 8;

  const gridDimensions: Array<u8> = [(screenDimensions[0] / gridSize as u16) as u8, (screenDimensions[1] / gridSize as u16) as u8];

  let x: u8 = 12;
  let y: u8 = 12;

  let playerBody: Array<Array<u8>> = [[0, 0]];
  let playerDirection = Direction.Right;
  let playerSize = 5;

  let frameCount: u8 = 0;

  let foodPos: Array<u8> = [nds.rand() % gridDimensions[0], nds.rand() % gridDimensions[1]];

  while (true) {
    if (nds.btnp(0) && playerDirection != Direction.Right) { playerDirection = Direction.Left; }
    if (nds.btnp(1) && playerDirection != Direction.Left) { playerDirection = Direction.Right; }
    if (nds.btnp(2) && playerDirection != Direction.Down) { playerDirection = Direction.Up; }
    if (nds.btnp(3) && playerDirection != Direction.Up) { playerDirection = Direction.Down; }

    // Game loop
    if (frameCount >= 6) {
      frameCount = 0;
      let playerHead = playerBody[0];

      // Eat food
      if (playerHead[0] == foodPos[0] && playerHead[1] == foodPos[1]) {
        playerSize += 5;
        foodPos = [nds.rand() % gridDimensions[0], nds.rand() % gridDimensions[1]];
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
          foodPos = [nds.rand() % gridDimensions[0], nds.rand() % gridDimensions[1]];
          break;
        }
      }
      if (playerHead[0] >= gridDimensions[0] || playerHead[1] >= gridDimensions[1]) {
        playerBody = [[0, 0]];
        playerDirection = Direction.Right;
        playerSize = 5;
        foodPos = [nds.rand() % gridDimensions[0], nds.rand() % gridDimensions[1]];
      }
    }

    // Rendering
    for (let s = 0; s < playerBody.length; s++) {
      let segment = playerBody[s];
      nds.fillRedRect(segment[0] * gridSize, segment[1] * gridSize, gridSize, gridSize);
    }

    nds.fillRedRect(foodPos[0] * gridSize, foodPos[1] * gridSize, gridSize, gridSize);

    frameCount++;

    nds.syncFrame();
  }
}

