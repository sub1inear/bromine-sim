const fs = require("fs");
const path = require("path");

const root = path.join(__dirname, "..");

fs.copyFileSync(path.join(root, "LICENSE"), path.join(__dirname, "LICENSE"));

function extractSection(lines, title) {
  let inFence = false;
  let start = -1;
  let level = 0;
  for (let i = 0; i < lines.length; i++) {
    if (/^\s*(```|~~~)/.test(lines[i])) {
      inFence = !inFence;
      continue;
    }
    if (inFence) continue;
    const match = /^(#{1,6})\s+(.*?)\s*$/.exec(lines[i]);
    if (!match) continue;
    if (start === -1) {
      if (match[2] === title) {
        start = i;
        level = match[1].length;
      }
    } else if (match[1].length <= level) {
      return lines.slice(start, i).join("\n").trimEnd();
    }
  }
  if (start === -1) {
    throw new Error(`Section "${title}" not found in README.md`);
  }
  return lines.slice(start).join("\n").trimEnd();
}

const mainLines = fs.readFileSync(path.join(root, "README.md"), "utf8").split(/\r?\n/);
const template = fs.readFileSync(path.join(__dirname, "README.template.md"), "utf8");
const readme = template.replace(/\{\{section:(.+?)\}\}/g, (_, title) => extractSection(mainLines, title.trim()));

fs.writeFileSync(path.join(__dirname, "README.md"), readme);
