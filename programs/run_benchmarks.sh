#!/bin/bash

emcc monte_carlo.c -s WASM=0 -o monte_carlo_asm.html
node --prof --log-timer-events --track_gc_object_stats --trace_gc_verbose --logfile=v8.%p.log monte_carlo.js

# For text representation
tick-processor $LOG_FILE
# OR for graphical representation
plot-timer-events $LOG_FILE
