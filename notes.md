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


## asm.js

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


A little more involved c program with a loop:

```
#include <stdio.h>

int main() {
  
  int counter = 0;
  for(int i=0;i<1000;i++){
  	counter += 1;
  }
  printf("%d\n",counter);  
  return counter;
}
```

Corresponding JS output:

```
function _main() {
 var $0 = 0, $1 = 0, $2 = 0, $3 = 0, $4 = 0, $5 = 0, $6 = 0, $7 = 0, $8 = 0, $9 = 0, $vararg_buffer = 0, label = 0, sp = 0;
 sp = STACKTOP;
 STACKTOP = STACKTOP + 16|0; if ((STACKTOP|0) >= (STACK_MAX|0)) abortStackOverflow(16|0);
 $vararg_buffer = sp;
 $0 = 0;
 $1 = 0;
 $2 = 0;
 while(1) {
  $3 = $2;
  $4 = ($3|0)<(1000);
  $5 = $1;
  if (!($4)) {
   break;
  }
  $6 = (($5) + 1)|0;
  $1 = $6;
  $7 = $2;
  $8 = (($7) + 1)|0;
  $2 = $8;
 }
 HEAP32[$vararg_buffer>>2] = $5;
 (_printf(384,$vararg_buffer)|0);
 $9 = $1;
 STACKTOP = sp;return ($9|0);
}
```

## LLVM optimizations

Let's try some LLVM optimizations. The JS code looks too verbose. Going through [documentation](http://kripken.github.io/emscripten-site/docs/tools_reference/emcc.html#emscripten-compiler-frontend-emcc) on `emcc` command, I see that there are multiple flags for optimizations:

* `-O0` -> No optimizations (default). This is the recommended setting for starting to port a project, as it includes various assertions.
* `-O1` -> Simple optimizations. These include using asm.js, LLVM -O1 optimizations, relooping, removing runtime assertions and C++ exception catching, and enabling `-s ALIASING_FUNCTION_POINTERS=1`. This is the recommended setting when you want a reasonably optimized build that is generated as quickly as possible (it builds much faster than `-O2`).

Running the optimization `O1` for the code above using command:

`./emcc -O1 tests/hello_world.c`

It outputs the following JS for the same loop C code:

```
function _main() {
 var $vararg_buffer = 0, label = 0, sp = 0;
 sp = STACKTOP;
 STACKTOP = STACKTOP + 16|0;
 $vararg_buffer = sp;
 HEAP32[$vararg_buffer>>2] = 1000;
 (_printf(380,$vararg_buffer)|0);
 STACKTOP = sp;return 1000;
}
```

## Execution pipeline of asm.js

I wanted to explore the pipeline of the entire execution of an `asm.js` project because that could give us a lot of intuition behind the optimizations they use. Browsers' JS runtime implicitly optimize JS execution using some tricks behind the scene. A little digging led me to the `run` function which is the first method that's called during the module execution.

This is what `run` looks like:

```
function run(args) {
  args = args || Module['arguments'];

  if (runDependencies > 0) {
    return;
  }

  writeStackCookie();

  preRun();

  if (runDependencies > 0) return; // a preRun added a dependency, run will be called later
  if (Module['calledRun']) return; // run may have just been called through dependencies being fulfilled just in this very frame

  function doRun() {
    if (Module['calledRun']) return; // run may have just been called while the async setStatus time below was happening
    Module['calledRun'] = true;

    if (ABORT) return;

    ensureInitRuntime();

    preMain();

    if (Module['onRuntimeInitialized']) Module['onRuntimeInitialized']();

    if (Module['_main'] && shouldRunNow) Module['callMain'](args);

    postRun();
  }

  if (Module['setStatus']) {
    Module['setStatus']('Running...');
    setTimeout(function() {
      setTimeout(function() {
        Module['setStatus']('');
      }, 1);
      doRun();
    }, 1);
  } else {
    doRun();
  }
  checkStackCookie();
}
```

The `callMain` method creates the stack and loads the `argv` and `argc` on stack.

```
 Module['callMain'] = function callMain(args) {
  assert(runDependencies == 0, 'cannot call main when async dependencies remain! (listen on __ATMAIN__)');
  assert(__ATPRERUN__.length == 0, 'cannot call main when preRun functions remain to be called');

  args = args || [];

  ensureInitRuntime();

  var argc = args.length+1;
  var argv = stackAlloc((argc + 1) * 4);
  HEAP32[argv >> 2] = allocateUTF8OnStack(Module['thisProgram']);
  for (var i = 1; i < argc; i++) {
    HEAP32[(argv >> 2) + i] = allocateUTF8OnStack(args[i - 1]);
  }
  HEAP32[(argv >> 2) + argc] = 0;


  try {

    var ret = Module['_main'](argc, argv, 0);


    // if we're not running an evented main loop, it's time to exit
      exit(ret, /* implicit = */ true);
  }
  catch(e) {
    if (e instanceof ExitStatus) {
      // exit() throws this once it's done to make sure execution
      // has been stopped completely
      return;
    } else if (e == 'SimulateInfiniteLoop') {
      // running an evented main loop, don't immediately exit
      Module['noExitRuntime'] = true;
      return;
    } else {
      var toLog = e;
      if (e && typeof e === 'object' && e.stack) {
        toLog = [e, e.stack];
      }
      Module.printErr('exception thrown: ' + toLog);
      Module['quit'](1, e);
    }
  } finally {
    calledMain = true;
  }
}
```

## Insights

And on further investigation, I notice this:

```
function updateGlobalBufferViews() {
  Module['HEAP8'] = HEAP8 = new Int8Array(buffer);
  Module['HEAP16'] = HEAP16 = new Int16Array(buffer);
  Module['HEAP32'] = HEAP32 = new Int32Array(buffer);
  Module['HEAPU8'] = HEAPU8 = new Uint8Array(buffer);
  Module['HEAPU16'] = HEAPU16 = new Uint16Array(buffer);
  Module['HEAPU32'] = HEAPU32 = new Uint32Array(buffer);
  Module['HEAPF32'] = HEAPF32 = new Float32Array(buffer);
  Module['HEAPF64'] = HEAPF64 = new Float64Array(buffer);
}
```

Insight : ASM uses Typed Array buffers as memory.

From Mozilla's docs on [Typed Arrays](https://developer.mozilla.org/en-US/docs/Web/JavaScript/Typed_arrays):
To achieve maximum flexibility and efficiency, JavaScript typed arrays split the implementation into buffers and views. A buffer (implemented by the ArrayBuffer object) is an object representing a chunk of data; it has no format to speak of and offers no mechanism for accessing its contents. In order to access the memory contained in a buffer, you need to use a view. A view provides a context — that is, a data type, starting offset, and the number of elements — that turns the data into a typed array.

![Array Buffer architecture](https://mdn.mozillademos.org/files/8629/typed_arrays.png)