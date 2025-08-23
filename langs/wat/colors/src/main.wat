
;; Simple test program that renders a square for each color in the palette

(module
  (import "env" "_clearScreen" (func $clear_screen (param i32)))
  (import "env" "_rectFill"    (func $rect_fill    (param i32 i32 i32 i32 i32)))

  (func $setup)

  (func $draw_rect (param $id i32)
    ;; x = 12 * id
    local.get $id
    i32.const 12
    i32.mul
    ;; y, w, h
    i32.const 0
    i32.const 12
    i32.const 12
    ;; color = id
    local.get $id
    call $rect_fill
  )

  (func $update
    ;; Clear screen to color 0
    i32.const 0
    call $clear_screen

    ;; Draw palette swatches
    i32.const 0
    call $draw_rect
    i32.const 1
    call $draw_rect
    i32.const 2
    call $draw_rect
    i32.const 3
    call $draw_rect
    i32.const 4
    call $draw_rect
    i32.const 5
    call $draw_rect
    i32.const 6
    call $draw_rect
    i32.const 7
    call $draw_rect
    i32.const 8
    call $draw_rect
    i32.const 9
    call $draw_rect
    i32.const 10
    call $draw_rect
    i32.const 11
    call $draw_rect
    i32.const 12
    call $draw_rect
    i32.const 13
    call $draw_rect
    i32.const 14
    call $draw_rect
    i32.const 15
    call $draw_rect
  )

  (export "setup" (func $setup))
  (export "update" (func $update))
)

