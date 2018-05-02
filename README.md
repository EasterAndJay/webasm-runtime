# webasm-runtime
Course Project for CS 263: Runtime Systems at UC Santa Barbara

## Setup

* Install rustup (also installs Rust compiler) `curl https://sh.rustup.rs -sSf | sh`
* Install Rust nightly for wasm compilation tools:
  * `rustup default nightly`
  * `rustup target add wasm32-unknown-unknown --toolchain nightly`
* Install a webserver for local development
  * `cargo install https`
* Start the webserver
  * `${HOME}.cargo/bin/http`

[Rust and WebAssembly Tutorial](https://rust-lang-nursery.github.io/rust-wasm/introduction.html)
