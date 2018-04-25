# WebAssembly notes

## Modules

The fundamental unit of WebAssembly code is a module.

A module is represented as a single S-Expression.

S-Expressions are a well known format for representing trees.

A module can be represented as a tree of nodes. This is similar to the concept of ASTs.

This is an example S-expression:

`(module (memory 1) (func))`

This means a module root node with a memory child node (attribute = 1) and a func child node.

## Functions

All code in modules are contained in functions. We represent a function as the following:

`( func <signature> <locals> <body> )`

Signature = typed params + return values
Locals = typed locals
Body = list of instructions

e.g.

```
(module
  (func (param $lhs i32) (param $rhs i32) (result i32)
    get_local $lhs
    get_local $rhs
    i32.add))
```

WebAssembly currently supports 4 types:

	*	i32: 32-bit integer
	*	i64: 64-bit integer
	*	f32: 32-bit float
	*	f64: 64-bit float

You can refer to locals, which are indexed from 0 and start with the parameters, with the `get_local` and `set_local` instructions. These instructions either take an index or a name (starts with $) to specify which local variable.

WebAssembly uses a stack machine. The stack is initially empty when a function is executed. You return values by leaving them on the stack. WebAssembly validates that you leave the corrects amount and type of variables on the stack according to the return values in the function signature.

You must export functions in order to call them. Like in an ES2015 module, wasm functions must be explicitly exported by an export statement inside the module.

Similar to how local variables work, functions are identified by an index by default, but for convenience, they can be named.

`(func $add … )`

`(export "add" (func $add))`

Here, “add” refers to the name used to call the function from JavaScript, while $add is specifying we want to export the function named $add in our WebAssembly module.

The above text representation of WebAssembly code is typically saved in. `.wat` file. You can convert this to binary format, `.wasm`, using `wabt`.

## JavaScript Interface

Here is how you would load the exported function from the WebAssembly binary using JavaScript:

```
WebAssembly.instantiateStreaming(fetch('add.wasm'))
.then(obj => {
   console.log(obj.instance.exports.add(1, 2));  // "3"
});
```

Not only can you import WebAssembly functions from JavaScript - the reverse works as well.

```
(module
  (import "console" "log" (func $log (param i32)))
  (func (export "logIt")
    i32.const 13
    call $log))
```

Here we import the JavaScript log function from the console module (i.e. console.log), and then we call that function in our own WebAssembly function, which we then export.

Interestingly, JavaScript functions have no notion of signature, so any JavaScript function can be passed, regardless of the import’s declared signature.

We need to pass a JavaScript object that houses all of our JS modules + function imports like so:

```
var importObject = {
  console: {
    log: function(arg) {
      console.log(arg);
    }
  }
};

WebAssembly.instantiateStreaming(fetch('logger.wasm'), importObject)
.then(obj => {
  obj.instance.exports.logIt();
});

```
## Memory

The low-level memory model of WebAssembly is represented as a contiguous range of untyped bytes. You read and write this memory, referrred to as Linear Memory, with load and store instructions.

Each WebAssembly instance accesses it's own instance of Linear Memory; you may have multiple webasm instances within one web app, which have isolated instances of Linear Memory.

WebAssembly Memory is measured in units of 64KB pages.

From JavaScript, you can construct an instance of WebAssembly Memory like so:

`var memory = new WebAssembly.Memory({initial:10, maximum:100});`

Here, the instance has 10 pages initially, with a max of 100 pages (6.4 MB).

Just like functions, linear memories can be defined inside a module or imported. Similarly, a module may also optionally export its memory. This means that JavaScript can get access to the memory of a WebAssembly instance either by creating a new `WebAssembly.Memory` and passing it in as an import or by receiving a Memory export (via `Instance.prototype.exports`).

Memory imports work just like function imports, only Memory objects are passed as values instead of JavaScript functions. Memory imports are useful for two reasons:

They allow JavaScript to fetch and create the initial contents of memory before or concurrently with module compilation.
They allow a single Memory object to be imported by multiple module instances, which is a critical building block for implementing dynamic linking in WebAssembly.

TODO:

- Tables section

