var deepcopy = function(o){
  return JSON.parse(JSON.stringify(o));
};
 
var src = {};
var head = src;

for(i = 0; i < 1000; i++) {
  src['next'] = {};
  src[i] = i;
  src = src['next'];
}
// print(JSON.stringify(src));
var dst = deepcopy(head);
// print(JSON.stringify(src));

