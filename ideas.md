# Idea log


## Web Assembly

[Web Assembly](http://webassembly.org/) is a compile target for the browsers that offers near native performance thereby becoming a very good compliment for Javascript. It is not necessary supposed to replace Javascript but it's developed so that developers can leverage low level languages like C, C++ and Rust to write critical components of a web app in form of Web Assembly (wasm) modules which can be easily imported in browsers. Browsers are adopting Web-Assembly more formally and wasm modules are on their way to become first class citizens in the JS world.

A very good introduction to WebAssembly and where it fits in is given in this 6 article series by Mozilla - https://hacks.mozilla.org/2017/02/a-cartoon-intro-to-webassembly/


## Asm.js

So WebAssembly can be perceived as an optimization for Javascript. Prior to WebAssembly, the alternative was asm.js

Asm.js is a highly optmized subset of Javascript and is not exactly a different programing language. [Spec](http://asmjs.org/spec/latest/)

It is therefore a very natural experiment to compare Web Assembly and asm.js and benchmark different aspects of performance.


It would also give us an insight on how the Javascript engine work with WebAssembly code. Different browsers have different JS engines and we could therefore also explore performance of WebAssembly across browsers and understand how they implement different optimizations


## Rust

Rust is one of the newest systems language in the block and offers the convenience and security of a high level language along with the performance benefits and memory management control provided by traditional low level languages like C and C++. Rust has an interesting notion of ownership too which helps address race conditions during data access. We could explore these features with respect to multi threaded code. So we can devise a bunch of experiments and benchmark how Rust performs in the face of parallel code.

I also found this extremely cool project on WebAssembly (https://github.com/wasdk/WebAssemblyStudio). I think they make use of a lot of interesting tools to create a playground for Web Assembly. 