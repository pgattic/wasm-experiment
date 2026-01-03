package main

import (
	"unsafe"
)

//go:wasm-module env
//export _rand
func _rand() uint32

//go:wasm-module env
//export _clearScreen
func _clearScreen(c uint8)

//go:wasm-module env
//export _pSet
func _pSet(x, y int32, c uint8)

//go:wasm-module env
//export _rect
func _rect(x, y int32, w, h uint32, c uint8)

//go:wasm-module env
//export _rectFill
func _rectFill(x, y int32, w, h uint32, c uint8)

//go:wasm-module env
//export _sprite
func _sprite(x, y int32, id uint8)

//go:wasm-module env
//export _btn
func _btn(btn uint8) bool

//go:wasm-module env
//export _btnP
func _btnP(btn uint8) bool

//go:wasm-module env
//export _printLnDbg
func _printLnDbg(ptr uintptr)


func rand() uint32 {
	return _rand()
}

func clearScreen(c uint8) {
	_clearScreen(c)
}

func pSet(x, y int32, c uint8) {
	_pSet(x, y, c)
}

func rect(x, y int32, w, h uint32, c uint8) {
	_rect(x, y, w, h, c)
}

func rectFill(x, y int32, w, h uint32, c uint8) {
	_rectFill(x, y, w, h, c)
}

func sprite(x, y int32, id uint8) {
	_sprite(x, y, id)
}

func btn(button Button) bool {
	return _btn(uint8(button))
}

func btnP(button Button) bool {
	return _btnP(uint8(button))
}

func print(str string) {
	// Allocate buffer and call debug print function
	data := []byte(str)
	_printLnDbg(uintptr(unsafe.Pointer(&data[0])))
}

type Button uint8

const (
	Left   Button = 0
	Right  Button = 1
	Up     Button = 2
	Down   Button = 3
	A      Button = 4
	B      Button = 5
	X      Button = 6
	Y      Button = 7
	L      Button = 8
	R      Button = 9
	Start  Button = 10
	Select Button = 11
)

func main() {}

