![Creating and working in assembly](https://hacks.mozilla.org/files/2017/02/04-03-toolchain07-768x631.png)

The compiler tool chain that currently has the most support for WebAssembly is called LLVM. There are a number of different front-ends and back-ends that can be plugged into LLVM.

Most WebAssembly module developers will code in languages like C and Rust and then compile to WebAssembly, but there are other ways to create a WebAssembly module.

Let’s say that we wanted to go from C to WebAssembly. 

We could use the clang front-end to go from C to the LLVM intermediate representation. Once it’s in LLVM’s IR, LLVM understands it, so LLVM can perform some optimizations.

To go from LLVM’s IR (intermediate representation) to WebAssembly, we need a back-end. 

There is one that’s currently in progress in the LLVM project. That back-end is most of the way there and should be finalized soon. However, it can be tricky to get it working today.

There’s another tool called Emscripten which is a bit easier to use at the moment. 

It has its own back-end that can produce WebAssembly by compiling to another target (called asm.js) and then converting that to WebAssembly. It uses LLVM under the hood, though, so you can switch between the two back-ends from Emscripten.

Emscripten includes many additional tools and libraries to allow porting whole C/C++ codebases, so it’s more of a software developer kit (SDK) than a compiler. 

For example, systems developers are used to having a filesystem that they can read from and write to, so Emscripten can simulate a file system using IndexedDB.

Regardless of the toolchain, the end result is a file that ends in `.wasm`. 

##Notes on LLVM:

The LLVM compiler infrastructure project is a "collection of modular and reusable compiler and toolchain technologies"[3] used to develop compiler front ends and back ends.

LLVM can provide the middle layers of a complete compiler system, taking intermediate representation (IR) code from a compiler and emitting an optimized IR. 

This new IR can then be converted and linked into machine-dependent assembly language code for a target platform. 

LLVM can accept the IR from the GNU Compiler Collection (GCC) toolchain, allowing it to be used with a wide array of extant compilers written for that project.

LLVM can also generate relocatable machine code at compile-time or link-time or even binary machine code at run-time.

LLVM supports a language-independent instruction set and type system. Each instruction is in static single assignment form (SSA), meaning that each variable (called a typed register) is assigned once and then frozen. This helps simplify the analysis of dependencies among variables. 

LLVM allows code to be compiled statically, as it is under the traditional GCC system, or left for late-compiling from the IR to machine code via just-in-time compilation (JIT), similar to Java. 

The type system consists of basic types such as integer or floating point numbers and five derived types: pointers, arrays, vectors, structures, and functions. 

A type construct in a concrete language can be represented by combining these basic types in LLVM. For example, a class in C++ can be represented by a mix of structures, functions and arrays of function pointers.

The LLVM JIT compiler can optimize unneeded static branches out of a program at runtime, and thus is useful for partial evaluation in cases where a program has many options, most of which can easily be determined unneeded in a specific environment.

## Evolution to WebAssembly

WebAssembly is a full fledged compile target but it started off as `asm.js` -  a spec led by the Mozilla team to formalize a subset of javascript. The great insight was that compiled javascript can be (a lot) faster than running regular js written by humans.


JS engines are getting faster and have become suitable for running large compiled codebases too but it is important to understand what kind of optimizations they employ under the hood to speed up JS performance.

The Emscripten project is based on these optimizations and is widely used to compile to JS.

The architecture is as follows:

C/C++ =========> LLVM =========> Emscripten =========> Javascript

Now I already did some research on LLVM and what it does. But the above representation gives us a good insight on how they smartly used LLVM's IR (also called LLVM bitcode) and based Emscripten off of that.

Emscripten takes the LLVM bitcode (with all its glorious optimizations) and then converts it into Javascript. I will do some more research on LLVM bitcode and  may be push some examples here too. But long story short, LLVM bitcode is like Java Bytecode (a semi-human readable Machine code representation). [The documentation](https://llvm.org/docs/BitCodeFormat.html#encoding-of-llvm-ir) describes `bitcode` as "a bitstream container format and an encoding of LLVM IR into the container format."

This architecture is very powerful. Since it can feed off of LLVM's bitcode, essentially any language that can be compiled to an LLVM bitcode representation can be compiled to Javascript! There are a lot of projects that explore translation of languages to C/C++ too which easily fits into the Emscripten toolchain.

Also found an excellent [list](https://github.com/jashkenas/coffeescript/wiki/List-of-languages-that-compile-to-JS) of languages that compile to JS. This list is impressive and growing everyday. WebAssembly only takes it further.

I decided to experiment on emscripten and see what JS it spits out. We can then think about what optimizations is LLVM and the Emscripten toolchain explicitly introducing and we can later look at benchmarking the results.

Let's try to run a basic `hello_world.c` program:

```
#include <stdio.h>

int main() {
  printf("Hello World\n");  
  return 0;
}
```

I compile (transpile) the code to JS using the following command:

`./emcc tests/hello_world.c`

This spits out an output file `a.out.js` which can be included in a browser or run directly using `node a.out.js`. The output JS file is huge (9026 lines to be exact) which seems quite a bit for a `hello world` program but it is important to appreciate that it has all the C bindings and essentially can take large game engines written in C and convert them to JS. So 9026 lines is quite a feat!

But this makes it extremely convoluted for me (humans) to make sense of the output. I will spend some time next week also getting into this file and understanding the different abstractions. All the optimizations live in this file, we just need to find them. After a lot of digging, I found the `main` function which was conveniently renamed `_main()`. It looks like:

```
function _main() {
 var $0 = 0, $vararg_buffer = 0, label = 0, sp = 0;
 sp = STACKTOP;
 STACKTOP = STACKTOP + 16|0; if ((STACKTOP|0) >= (STACK_MAX|0)) abortStackOverflow(16|0);
 $vararg_buffer = sp;
 $0 = 0;
 (_printf(384,$vararg_buffer)|0);
 STACKTOP = sp;return 0;
}
```

I know right? Where is the string "Hello World"? Spoiler alert it is no where in the code. Ofcoure there are a lot of abstractions and the characters are encoded as their codes and somehow find their way into the `$vararg_buffer` or the number `384`. It seems like the `$vararg_buffer` is supposed to contain the arguments and is picked up from the top of stack.