import fs from "fs";
import path from "path";
import { fileURLToPath } from "url";
import { parse } from "/Users/frgonzal/.npm/_npx/10f54a790ecaa614/node_modules/markdownlint/lib/micromark-parse.mjs";
import { filterByPredicate, filterByTypes } from "/Users/frgonzal/.npm/_npx/10f54a790ecaa614/node_modules/markdownlint/helpers/micromark-helpers.cjs";

const __dirname = path.dirname(fileURLToPath(import.meta.url));
const docsDir = path.join(__dirname, "..", "docs");

const childrenExclude = new Set([ "image", "reference", "resource" ]);
const tokensInclude = new Set(
  [ "characterEscapeValue", "codeTextData", "data", "mathTextData" ]
);

function getHeadingFragment(headingTextToken) {
  const inlineText = filterByPredicate(
    headingTextToken.children,
    (token) => tokensInclude.has(token.type),
    (token) => (childrenExclude.has(token.type) ? [] : token.children)
  )
    .map((token) => token.text)
    .join("");
  
  return "#" + encodeURIComponent(
    inlineText
      .toLowerCase()
      .replace(
        /[^\p{Letter}\p{Mark}\p{Number}\p{Connector_Punctuation}\- ]/gu,
        ""
      )
      .replace(/ /gu, "-")
      .toWellFormed()
  );
}

const files = fs.readdirSync(docsDir).filter(f => f.endsWith(".md")).sort();

for (const file of files) {
  const filePath = path.join(docsDir, file);
  let content = fs.readFileSync(filePath, "utf8");

  // 1. Fix duplicate headings in Module 17
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

  // 2. Fix code blocks missing language (MD040)
  let inCode = false;
  let lines = content.split("\n");
  let fixedLines = [];
  for (const line of lines) {
    const trimmed = line.trim();
    if (trimmed.startsWith("```")) {
      if (!inCode) {
        inCode = true;
        const lang = trimmed.slice(3).trim();
        if (!lang) {
          fixedLines.push("```text");
        } else {
          fixedLines.push(line);
        }
      } else {
        inCode = false;
        fixedLines.push(line);
      }
    } else {
      fixedLines.push(line);
    }
  }
  content = fixedLines.join("\n");

  // 3. Fix list numbering in References section (MD029)
  lines = content.split("\n");
  fixedLines = [];
  let inRefSub = false;
  let refCount = 0;
  for (const line of lines) {
    if (line.startsWith("### Official") || line.startsWith("### Authoritative")) {
      inRefSub = true;
      refCount = 0;
      fixedLines.push(line);
      continue;
    } else if (inRefSub && (line.startsWith("## ") || line.startsWith("---"))) {
      inRefSub = false;
      refCount = 0;
      fixedLines.push(line);
      continue;
    }

    if (inRefSub) {
      const match = line.match(/^(\d+)\.(\s+\[.*)/);
      if (match) {
        refCount++;
        fixedLines.push(`${refCount}.${match[2]}`);
        continue;
      }
    }
    fixedLines.push(line);
  }
  content = fixedLines.join("\n");

  // 4. Ensure blanks around headings and lists (MD022, MD032)
  lines = content.split("\n");
  fixedLines = [];
  inCode = false;
  for (let i = 0; i < lines.length; i++) {
    const line = lines[i];
    const trimmed = line.trim();

    if (trimmed.startsWith("```")) {
      inCode = !inCode;
      fixedLines.push(line);
      continue;
    }

    if (inCode) {
      fixedLines.push(line);
      continue;
    }

    // Heading
    if (line.startsWith("#")) {
      // Ensure blank line before
      if (fixedLines.length > 0 && fixedLines[fixedLines.length - 1].trim() !== "" && !fixedLines[fixedLines.length - 1].startsWith("---")) {
        fixedLines.push("");
      }
      
      // Remove trailing colon
      const cleanHeading = line.replace(/^(#+.*?):\s*$/, "$1");
      fixedLines.push(cleanHeading);
      continue;
    }

    // List item
    const isListItem = /^(\*|\-|\d+\.)\s+/.test(trimmed);
    if (isListItem) {
      if (fixedLines.length > 0 && fixedLines[fixedLines.length - 1].trim() !== "") {
        const prevTrimmed = fixedLines[fixedLines.length - 1].trim();
        const prevIsList = /^(\*|\-|\d+\.)\s+/.test(prevTrimmed);
        if (!prevIsList) {
          fixedLines.push("");
        }
      }
      fixedLines.push(line);
      continue;
    }

    // Normal text following heading
    if (fixedLines.length > 0 && fixedLines[fixedLines.length - 1].startsWith("#") && trimmed !== "") {
      fixedLines.push("");
    }

    fixedLines.push(line);
  }
  content = fixedLines.join("\n");

  // 5. Rebuild Table of Contents using exact micromark tokens (MD051)
  if (file !== "index.md") {
    // Parse tokens to get exact fragment slugs
    const docToken = parse(content);
    const headingTokens = filterByTypes(docToken, [ "atxHeadingText", "setextHeadingText" ]);

    const tocEntries = [];
    let tocIndex = 1;
    for (const ht of headingTokens) {
      // Find level 2 section headings starting with a number e.g. "1. High-Level...", "2. Process..."
      const match = ht.text.match(/^(\d+)\.\s+(.*)$/);
      if (match) {
        const title = match[2].replace(/[`]/g, "").replace(/<[^>]+>/g, "");
        const fragment = getHeadingFragment(ht);
        tocEntries.push(`${tocIndex}. [${title}](${fragment})`);
        tocIndex++;
      }
    }

    const tocStr = tocEntries.join("\n");
    const tocPattern = /(##\s+📑\s+Table of Contents\s*\n+)([\s\S]*?)(?=\n+---\s*\n+|\n+## |\Z)/;
    if (tocPattern.test(content)) {
      content = content.replace(tocPattern, `## 📑 Table of Contents\n\n${tocStr}\n`);
    }
  }

  // 6. Clean up multiple blank lines (MD012)
  content = content.replace(/\n{3,}/g, "\n\n");
  if (!content.endsWith("\n")) {
    content += "\n";
  }

  fs.writeFileSync(filePath, content, "utf8");
}

console.log("Successfully synced all markdown files.");
