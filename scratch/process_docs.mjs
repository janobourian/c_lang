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

  // Fix MD050: __VA_ARGS__, __asm__, __volatile__ in headings or text -> wrap in code spans
  content = content.replace(/__VA_ARGS__/g, "`__VA_ARGS__`");
  content = content.replace(/__asm__/g, "`__asm__`");
  content = content.replace(/__volatile__/g, "`__volatile__`");
  content = content.replace(/``__VA_ARGS__``/g, "`__VA_ARGS__`");
  content = content.replace(/``__asm__``/g, "`__asm__`");
  content = content.replace(/``__volatile__``/g, "`__volatile__`");
  content = content.replace(/\*\*`__asm__`\*\*/g, "`__asm__`");
  content = content.replace(/\*\*`__volatile__`\*\*/g, "`__volatile__`");

  // Fix HTML header lookalikes in headings: <stdio.h> -> `<stdio.h>`
  content = content.replace(/^(#+\s+.*?)<([a-zA-Z0-9_\-\.]+)>([^\n]*)$/gm, "$1`<$2>`$3");

  // Fix MD024: Duplicate headings in 17
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

  // Fix MD004: Unordered list style (dashes to asterisks)
  content = content.replace(/^(\s*)-\s+/gm, "$1* ");

  // Fix MD040: Fenced code block languages
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

  // Fix MD029: Ordered list item prefix in References section
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

  // Fix MD022, MD031, MD032: Blanks around headings, code fences, and lists
  lines = content.split("\n");
  fixedLines = [];
  inCode = false;

  for (let i = 0; i < lines.length; i++) {
    const line = lines[i];
    // Remove single trailing space (MD009)
    const cleanedLine = line.replace(/([^\s])\s$/, "$1");
    const trimmed = cleanedLine.trim();

    if (trimmed.startsWith("```")) {
      if (!inCode) {
        if (fixedLines.length > 0 && fixedLines[fixedLines.length - 1].trim() !== "" && !fixedLines[fixedLines.length - 1].startsWith("---")) {
          fixedLines.push("");
        }
        inCode = true;
        fixedLines.push(cleanedLine);
      } else {
        inCode = false;
        fixedLines.push(cleanedLine);
      }
      continue;
    }

    if (inCode) {
      fixedLines.push(cleanedLine);
      continue;
    }

    // Heading
    if (cleanedLine.startsWith("#")) {
      if (fixedLines.length > 0 && fixedLines[fixedLines.length - 1].trim() !== "" && !fixedLines[fixedLines.length - 1].startsWith("---")) {
        fixedLines.push("");
      }
      const cleanHeading = cleanedLine.replace(/^(#+.*?):\s*$/, "$1");
      fixedLines.push(cleanHeading);
      continue;
    }

    // List item (* or numbered)
    const isListItem = /^(\*|\d+\.)\s+/.test(trimmed);
    if (isListItem) {
      if (fixedLines.length > 0 && fixedLines[fixedLines.length - 1].trim() !== "") {
        const prevTrimmed = fixedLines[fixedLines.length - 1].trim();
        const prevIsList = /^(\*|\d+\.)\s+/.test(prevTrimmed);
        if (!prevIsList) {
          fixedLines.push("");
        }
      }
      fixedLines.push(cleanedLine);
      continue;
    }

    // Normal text following a heading or closing fence without blank line
    if (fixedLines.length > 0) {
      const prevLine = fixedLines[fixedLines.length - 1];
      if ((prevLine.startsWith("#") || prevLine.startsWith("```")) && trimmed !== "") {
        fixedLines.push("");
      }
    }

    fixedLines.push(cleanedLine);
  }
  content = fixedLines.join("\n");

  // Fix MD038: spaces in code spans
  content = content.replace(/`\s+`\s+/g, "` `");
  content = content.replace(/`\s+\\/g, "`\\");

  // Rebuild TOC cleanly (MD051) for Level 2 headings only
  const h2Lines = [];
  for (const l of content.split("\n")) {
    const m = l.match(/^##\s+(\d+\.\s+.*)$/);
    if (m) {
      h2Lines.push(m[1]);
    }
  }

  const docToken = parse(content);
  const headingTokens = filterByTypes(docToken, [ "atxHeadingText", "setextHeadingText" ]);

  const tocEntries = [];
  let tocIndex = 1;
  for (const h2 of h2Lines) {
    const match = h2.match(/^(\d+)\.\s+(.*)$/);
    if (match) {
      const title = match[2].replace(/[`]/g, "");
      // Find matching heading token
      const token = headingTokens.find(ht => ht.text.trim() === h2.trim() || ht.text.replace(/[`]/g, "").trim() === h2.replace(/[`]/g, "").trim());
      if (token) {
        const fragment = getHeadingFragment(token);
        tocEntries.push(`${tocIndex}. [${title}](${fragment})`);
        tocIndex++;
      }
    }
  }

  const tocStr = tocEntries.join("\n");
  const tocPattern = /(##\s+📑\s+Table of Contents\s*\n+)[\s\S]*?(?=\n+---\s*\n+)/;
  if (tocPattern.test(content)) {
    content = content.replace(tocPattern, `## 📑 Table of Contents\n\n${tocStr}\n`);
  }

  // Clean up extra blank lines (MD012)
  content = content.replace(/\n{3,}/g, "\n\n");
  if (!content.endsWith("\n")) {
    content += "\n";
  }

  fs.writeFileSync(filePath, content, "utf8");
}

console.log("Processing complete.");
