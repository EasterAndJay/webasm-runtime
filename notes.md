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