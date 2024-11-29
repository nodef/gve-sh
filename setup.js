'use strict';
const cp = require('child_process');
const fs = require('fs');
const os = require('os');


// Read a text file.
function readFile(pth) {
  var d = fs.readFileSync(pth, 'utf8');
  return d.replace(/\r?\n|\r/g, '\n');
}


// Write a text file.
function writeFile(pth, txt) {
  var d = txt.replace(/\r?\n|\r/g, os.EOL);
  fs.writeFileSync(pth, d);
}


// Main function.
function main() {
  var p   = JSON.parse(readFile('package.json'));
  var env = Object.assign({DOWNLOAD: '0', RUN: '0', VERSION: p.version}, process.env);
  cp.execSync(`bash main.sh`, {stdio: 'inherit', env});
  cp.execSync(`rm -rf inc/ *.hxx *.cxx`, {stdio: 'inherit'});
  cp.execSync(`rm -f main.sh`, {stdio: 'inherit'});
  cp.execSync(`mv a.out main.sh`, {stdio: 'inherit'});
}
main();
