;; Simple WASM Text file that renders a sprite.
;; You DO NOT have to know how to write this code to use the engine, and are encouraged to use your favorite programming language instead :)
;; See https://github.com/pgattic/wasm-experiment/blob/master/langs/README.md

(module
  ;; Import the engine functions I'll use
  (import "env" "_sprite" (func $sprite (param i32 i32 i32)))

  ;; Export setup and update functions to the engine
  (export "setup" (func $setup))
  (export "update" (func $update))

  (func $setup nop)

  (func $update
    ;; Place parameters on stack
    i32.const 50 ;; x
    i32.const 50 ;; y
    i32.const 1  ;; sprite_id
    ;; Invoke `sprite` function (consumes the three items)
    call $sprite
  )
)

;; Want to learn more about hand-writing WASM code? Check out this introduction:
;; https://developer.mozilla.org/en-US/docs/WebAssembly/Guides/Understanding_the_text_format

