'use strict';
const cp = require('child_process');
const fs = require('fs');


/**
 * Read a text file.
 * @param {string} pth file path
 * @returns {string} file content
 */
function readFile(pth) {
  var d = fs.readFileSync(pth, 'utf8');
  return d.replace(/\r?\n|\r/g, '\n');
}


/**
 * Main function.
 */
function main() {
  var msg = process.argv[2];
  // Check if the commit message includes '[publish]' and the NPM_TOKEN environment variable is set.
  if (!msg.includes('[publish]')) { console.log('Please include "[publish]" in your commit message'); return; }
  if (!process.env.NPM_TOKEN)     { console.log('Please set the NPM_TOKEN environment variable'); return; }
  // Publish the package.
  var m   = JSON.parse(readFile('package.json'));
  console.log(`Publish of ${m.name} v${m.version} triggered by commit message: ${msg}`);
  cp.execSync('npm publish', {stdio: 'inherit'});
}
main();
