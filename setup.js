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
  var env = Object.assign({DOWNLOAD: '0', RUN: '0'}, process.env);
  cp.execSync(`bash main.sh`, {stdio: 'inherit', env});
  fs.unlinkSync('main.sh');
  fs.renameSync('a.out', 'main.sh');
  cp.execSync(`rm -rf inc/ *.hxx *.cxx`, {stdio: 'inherit'});
}
main();
