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

export function getHeadingFragments(content) {
  const documentToken = parse(content);
  const headingTexts = filterByTypes(documentToken, [ "atxHeadingText", "setextHeadingText" ]);
  const fragments = [];
  
  for (const headingText of headingTexts) {
    const inlineText = filterByPredicate(
      headingText.children,
      (token) => tokensInclude.has(token.type),
      (token) => (childrenExclude.has(token.type) ? [] : token.children)
    )
      .map((token) => token.text)
      .join("");
    
    const fragment = "#" + encodeURIComponent(
      inlineText
        .toLowerCase()
        .replace(
          /[^\p{Letter}\p{Mark}\p{Number}\p{Connector_Punctuation}\- ]/gu,
          ""
        )
        .replace(/ /gu, "-")
        .toWellFormed()
    );
    fragments.push({
      headingText: headingText.text,
      inlineText,
      fragment
    });
  }
  return fragments;
}

const testContent = fs.readFileSync(path.join(docsDir, "00_c_foundations_syntax_types_and_control_flow.md"), "utf8");
console.log(getHeadingFragments(testContent));
