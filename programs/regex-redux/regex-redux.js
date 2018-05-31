// The Computer Language Benchmarks Game
// https://salsa.debian.org/benchmarksgame-team/benchmarksgame/
//
// regex-dna program contributed by Jesse Millikan
// Base on the Ruby version by jose fco. gonzalez
// fixed by Matthew Wilson
// ported to Node.js and sped up by Roman Pletnev
// converted from regex-dna program
// fixed by Josh Goldfoot

var fs = require('fs'), i = fs.readFileSync('/dev/stdin', 'ascii'),
  ilen = i.length, clen, j,
  q = [/agggtaaa|tttaccct/ig, /[cgt]gggtaaa|tttaccc[acg]/ig,
    /a[act]ggtaaa|tttacc[agt]t/ig, /ag[act]gtaaa|tttac[agt]ct/ig,
    /agg[act]taaa|ttta[agt]cct/ig, /aggg[acg]aaa|ttt[cgt]ccct/ig,
    /agggt[cgt]aa|tt[acg]accct/ig, /agggta[cgt]a|t[acg]taccct/ig,
    /agggtaa[cgt]|[acg]ttaccct/ig],

i = i.replace(/^>.*\n|\n/mg, '');
clen = i.length;
for(j = 0; j<q.length; ++j) {
  var qj = q[j], m = i.match(qj);
  console.log(qj.source, m ? m.length : 0);
}

i = i.replace(/tHa[Nt]/g, "<4>")
     .replace(/aND|caN|Ha[DS]|WaS/g, "<3>")
     .replace(/a[NSt]|BY/g, "<2>")
     .replace(/<[^>]*>/g, "|")
     .replace(/\|[^|][^|]*\|/g, "-");

console.log(["", ilen, clen, i.length].join("\n"));