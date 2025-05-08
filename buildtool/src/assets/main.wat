;; Sample `main.wat` file that renders a sprite.
;; You DO NOT have to know how to write this code to use the engine, and are encouraged to use your favorite language instead :)
;; See https://github.com/pgattic/wasm-experiment/blob/master/langs/README.md

(module
  ;; Import the engine functions I'll use
  (import "env" "_sprite" (func $sprite (param i32 i32 i32)))

  ;; Export setup and update functions to the engine
  (export "setup" (func $setup))
  (export "update" (func $update))

  (func $setup
    nop
  )

  (func $update
    i32.const 50 ;; Place parameters on stack
    i32.const 50
    i32.const 1
    call $sprite ;; Invoke `sprite` function (consumes the three items)
  )
)

