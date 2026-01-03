package main

//export setup
func setup() {
	print("Hello from Golang!")
}

//export update
func update() {
	rectFill(12, 12, 24, 24, 2)
}

