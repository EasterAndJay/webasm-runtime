# Spectral Norm
Test cases

type, input,Evaluate script, compile script
org,10,14.3,12.1,0.9
asm,10,162.5,131.3,29.9
org,100,27.1,24.4,0.6
asm,100,4120.8,4079.7,39.7

# Monte Carlo

type, input,Evaluate script, compile script
org,10^6,129.7,127.1,0.6
asm,10^6,61.2,29.7,30.3
wasm,10^6,22.5,18.9,1.8
org,10^9,86599.2,86588.0,0.6
asm,10^9,42.5,21.5,19.2
wasm,10^9,13.4,6.7,4.2
org,10^8,8795,8792.1,0.9
asm,10^8,47.4,18.4,27.0
wasm,10^8,7.8,2.4,2.4

# Deepcopy

type, input,Evaluate script, compile script
org,10^4,83.0,28.7
asm,10^4,105.7,37.3
wasm,10^4,119.0,43.4
org,10^5,169.8,32.4
asm,10^5,110.1,37.2
wasm,10^5,103.4,42.9
org,10^6,636.6,31.5
asm, 10^6,130.1,37.9
wasm, 10^6,107.0,38.0 
