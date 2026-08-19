const fs = require("fs");
const path = require("path");

const docsDir = path.join(__dirname, "..", "docs");

function convertHeadingToSlug(headingText) {
  // Remove markdown link syntax [text](url) -> text
  let text = headingText.replace(/\[([^\]]+)\]\([^\)]+\)/g, "$1");
  // Remove backticks
  text = text.replace(/`/g, "");
  // If there are unescaped html tags like <stdint.h>, micromark treats them as html tags and drops them
  // But if written as `<stdint.h>` in heading, text is <stdint.h>
  text = text.replace(/<[^>]+>/g, "");
  
  return encodeURIComponent(
    text
      .toLowerCase()
      .replace(/[^\p{Letter}\p{Mark}\p{Number}\p{Connector_Punctuation}\- ]/gu, "")
      .replace(/ /gu, "-")
  );
}

const files = fs.readdirSync(docsDir).filter(f => f.endsWith(".md")).sort();

for (const file of files) {
  const filePath = path.join(docsDir, file);
  let content = fs.readFileSync(filePath, "utf8");

  // Fix MD024 for Module 17
  if (file === "17_real_world_enterprise_case_studies_and_hands_on.md") {
    let count = 0;
    content = content.replace(/^### Architectural Invariants:?$/gm, () => {
      count++;
      if (count === 1) return "### Architectural Invariants: Financial Order Matching Engine";
      if (count === 2) return "### Architectural Invariants: Industrial IoT Telemetry & MMIO Sensor Daemon";
      if (count === 3) return "### Architectural Invariants: High-Throughput Epoll HTTP & Static Asset Server";
      return `### Architectural Invariants: Subsystem ${count}`;
    });
  }

  // Find all ## section headings for TOC
  if (file !== "index.md") {
    const lines = content.split("\n");
    const h2Headings = [];
    for (const line of lines) {
      const m = line.match(/^##\s+(\d+\.\s+.*)$/);
      if (m) {
        h2Headings.push(m[1]);
      }
    }

    const tocEntries = [];
    for (const h of h2Headings) {
      const match = h.match(/^(\d+)\.\s+(.*)$/);
      if (match) {
        const num = match[1];
        const title = match[2];
        const slug = convertHeadingToSlug(h);
        const cleanTitle = title.replace(/[`]/g, "");
        tocEntries.push(`${num}. [${cleanTitle}](#${slug})`);
      }
    }

    const tocStr = tocEntries.join("\n");
    const tocPattern = /(##\s+📑\s+Table of Contents\s*\n)([\s\S]*?)(?=\n---|\n## |\Z)/;
    if (tocPattern.test(content)) {
      content = content.replace(tocPattern, `$1\n${tocStr}\n`);
    }
  }

  // In index.md, fix any TOC links if present
  if (file === "index.md") {
    const lines = content.split("\n");
    const h2Headings = [];
    for (const line of lines) {
      const m = line.match(/^##\s+(\d+\.\s+.*)$/);
      if (m) {
        h2Headings.push(m[1]);
      }
    }

    const tocEntries = [];
    for (const h of h2Headings) {
      const match = h.match(/^(\d+)\.\s+(.*)$/);
      if (match) {
        const num = match[1];
        const title = match[2];
        const slug = convertHeadingToSlug(h);
        const cleanTitle = title.replace(/[`]/g, "");
        tocEntries.push(`${num}. [${cleanTitle}](#${slug})`);
      }
    }

    const tocStr = tocEntries.join("\n");
    const tocPattern = /(##\s+📑\s+Table of Contents\s*\n)([\s\S]*?)(?=\n---|\n## |\Z)/;
    if (tocPattern.test(content)) {
      content = content.replace(tocPattern, `$1\n${tocStr}\n`);
    }
  }

  fs.writeFileSync(filePath, content, "utf8");
}

console.log("TOC synchronization complete.");
