cargo build --target wasm32-unknown-unknown --release && \
xxd -i target/wasm32-unknown-unknown/release/nds_rust.wasm > app.wasm.h
