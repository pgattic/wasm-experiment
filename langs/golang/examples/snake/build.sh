OUTPUT=snake.wasm

tinygo build -o $OUTPUT -target=wasm -opt=z -no-debug main.go

