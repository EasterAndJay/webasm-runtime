#!/bin/bash


source "$HOME/code/school/cs263/emsdk/emsdk_env.sh"

declare -a langs=("js" "asm" "wasm")

for dir in */
do
  dir=${dir%/}
  echo $dir
  # Compile asm + wasm
  emcc "$dir/$dir.c" -s WASM=0 -o "$dir/$dir_asm.html"
  emcc "$dir/$dir.c" -s WASM=1 -o "$dir/$dir_wasm.html"
  for lang in langs; do
    if [ $lang == "js" ]; then
      js_file="$dir/$dir.js"
    else
      js_file="$dir/$dir_$lang.js"
    fi
    for ((i=1; i<=100; i++)); do
      log_file="$dir/data/$lang_$1.v8.log"
      
      # echo "Profiling attempt: $i for $js_file"
      # node --prof --log-timer-events --track_gc_object_stats --trace_gc_verbose --logfile=$log_file $js_file 
      # tick-processor $log_file
      # Collect metrics from $log_file
    done
  done
done

# For text representation
# tick-processor $LOG_FILE
# OR for graphical representation
# plot-timer-events $LOG_FILE
