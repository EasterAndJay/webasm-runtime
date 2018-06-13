# webasm-runtime
Course Project for CS 263: Runtime Systems at UC Santa Barbara

## Final Presentation
Uploaded as `CS 263 - WebAssembly.pdf` in this repo.
Google drive Read only link - https://docs.google.com/presentation/d/1oc4ynTOwDnZeJlmJqLB0oggZRxxQ7RxIIhwIAto2I28/edit?usp=sharing


## Setup and run experiments

Please have Node.js installed or Google Chrome

* To run in Google Chrome, navigate to the respective program directory (e.g. `programs/monte_carlo`).
* Then open the corresponding html file (${program_name}_{org,asm,wasm}.html) in Chrome.
  *  NOTE: `org` stands for 'original', which corresponds to javascript.
* Then open up the Javascript console in Chrome.
* Navigate to the `Performance` tab.
* Click the circular arrow icon in the top left of the console to begin profiling.
* Click the `Call Tree` tab and then `Evaluate Script` to see the script evaluation and compilation times.

NOTE: Node.js is a bit more complicated to produce interpretable results

* You need to install v8 and the v8 profiler (follow instructions here: https://github.com/v8/v8/wiki/V8-Profiler)
* Run one of the commands at the bottom of this README
* Each command produces a v8 tick log
* You can then use the appropriate tick-processor mentioned in the link above to transform the tick log to profiler results.

`node --prof --log-timer-events --track_gc_object_stats --trace_gc_verbose monte_carlo.js`

`node --prof --log-timer-events --track_gc_object_stats --trace_gc_verbose monte_carlo_asm.js`

`node --prof --log-timer-events --track_gc_object_stats --trace_gc_verbose monte_carlo_wasm.js`

`node --prof --log-timer-events --track_gc_object_stats --trace_gc_verbose spectral-norm.js 100`

`node --prof --log-timer-events --track_gc_object_stats --trace_gc_verbose spectral-norm_asm.js 100`

`node --prof --log-timer-events --track_gc_object_stats --trace_gc_verbose deepcopy.js`

`node --prof --log-timer-events --track_gc_object_stats --trace_gc_verbose deepcopy_asm.js`

`node --prof --log-timer-events --track_gc_object_stats --trace_gc_verbose deepcopy_wasm.js`
