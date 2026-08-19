#!/usr/bin/env python3
import os
import re
import glob
import subprocess
import json

docs_dir = "/Users/frgonzal/Documents/maxine/c_lang/docs"

def github_slug(text):
    # Remove leading hashes
    text = re.sub(r"^#+\s*", "", text)
    # Remove markdown links [text](url) -> text
    text = re.sub(r"\[([^\]]+)\]\([^\)]+\)", r"\1", text)
    # Remove inline code backticks, bold, italic
    text = re.sub(r"[`\*_]", "", text)
    # Remove math expressions $...$
    text = re.sub(r"\$[^$]+\$", "", text)
    # Remove html tags
    text = re.sub(r"<[^>]+>", "", text)
    # Lowercase
    text = text.lower()
    # Remove all characters except alphanumeric, whitespace, and hyphens
    text = re.sub(r"[^\w\s\-]", "", text)
    # Replace any whitespace character with hyphen
    text = re.sub(r"\s+", "-", text)
    return text.strip("-")

def fix_file(filepath):
    with open(filepath, "r", encoding="utf-8") as f:
        lines = f.readlines()
    
    filename = os.path.basename(filepath)
    print(f"Processing {filename}...")

    # Fix 1: MD024 in Module 17
    if filename == "17_real_world_enterprise_case_studies_and_hands_on.md":
        inv_count = 0
        new_lines = []
        for line in lines:
            if line.strip() == "### Architectural Invariants:":
                inv_count += 1
                if inv_count == 1:
                    new_lines.append("### Architectural Invariants: Financial Order Matching Engine\n")
                elif inv_count == 2:
                    new_lines.append("### Architectural Invariants: Industrial IoT Telemetry & MMIO Sensor Daemon\n")
                elif inv_count == 3:
                    new_lines.append("### Architectural Invariants: High-Throughput Epoll HTTP & Static Asset Server\n")
                else:
                    new_lines.append(f"### Architectural Invariants: Subsystem {inv_count}\n")
            else:
                new_lines.append(line)
        lines = new_lines

    # Fix 2: MD029 (Ordered list item prefix in References)
    new_lines = []
    ref_sub_count = 0
    in_ref_sub = False
    for line in lines:
        if line.startswith("### Official") or line.startswith("### Authoritative"):
            in_ref_sub = True
            ref_sub_count = 0
            new_lines.append(line)
            continue
        elif in_ref_sub and line.startswith("##"):
            in_ref_sub = False
            ref_sub_count = 0
            new_lines.append(line)
            continue

        if in_ref_sub:
            m = re.match(r"^(\d+)\.(\s+\[.*)", line)
            if m:
                ref_sub_count += 1
                new_lines.append(f"{ref_sub_count}.{m.group(2)}\n")
                continue
        new_lines.append(line)
    lines = new_lines

    # Fix 3: MD040 (Fenced code block language missing)
    in_code_block = False
    new_lines = []
    for line in lines:
        stripped = line.strip()
        if stripped.startswith("```"):
            if not in_code_block:
                in_code_block = True
                lang = stripped[3:].strip()
                if not lang:
                    new_lines.append("```text\n")
                else:
                    new_lines.append(line)
            else:
                in_code_block = False
                new_lines.append(line)
        else:
            new_lines.append(line)
    lines = new_lines

    # Fix 4: MD022 (blanks around headings) & MD032 (blanks around lists)
    new_lines = []
    in_code_block = False
    for i, line in enumerate(lines):
        stripped = line.strip()
        if stripped.startswith("```"):
            in_code_block = not in_code_block
            new_lines.append(line)
            continue

        if in_code_block:
            new_lines.append(line)
            continue

        # If heading
        if line.startswith("#"):
            if new_lines and new_lines[-1].strip() != "" and not new_lines[-1].startswith("---"):
                new_lines.append("\n")
            
            # Remove trailing colon if any (MD026)
            fixed_heading = re.sub(r"^(#+.*?):\s*$", r"\1\n", line)
            new_lines.append(fixed_heading)
            continue

        # If list item (*, -, 1., etc.) following directly after a non-list, non-blank line
        is_list_item = bool(re.match(r"^(\*|\-|\d+\.)\s+", stripped))
        if is_list_item:
            if new_lines and new_lines[-1].strip() != "":
                prev_stripped = new_lines[-1].strip()
                prev_is_list = bool(re.match(r"^(\*|\-|\d+\.)\s+", prev_stripped))
                if not prev_is_list:
                    new_lines.append("\n")
            new_lines.append(line)
            continue

        # If normal text line directly following a heading without blank line
        if new_lines and new_lines[-1].startswith("#") and stripped != "":
            new_lines.append("\n")

        new_lines.append(line)

    lines = new_lines

    # Fix 5: MD051 (Rebuild Table of Contents based on actual ## headings)
    if filename != "index.md":
        content = "".join(lines)
        
        # Collect all ## headings
        h2_headings = []
        for line in lines:
            if re.match(r"^##\s+\d+\.\s+", line):
                h2_headings.append(line.strip()[3:]) # remove "## "
        
        toc_entries = []
        for h in h2_headings:
            m = re.match(r"^(\d+)\.\s+(.*)$", h)
            if m:
                num = m.group(1)
                title = m.group(2)
                slug = github_slug(f"## {h}")
                clean_title = re.sub(r"[`]", "", title)
                toc_entries.append(f"{num}. [{clean_title}](#{slug})")
        
        toc_str = "\n".join(toc_entries)
        
        toc_match = re.search(r"(##\s+📑\s+Table of Contents\s*\n)(.*?)(?=\n---|\n## |\Z)", content, re.DOTALL)
        if toc_match:
            start_pos, end_pos = toc_match.span()
            header_prefix = toc_match.group(1)
            content = content[:start_pos] + header_prefix + "\n" + toc_str + "\n" + content[end_pos:]
            lines = [l + "\n" for l in content.splitlines()]

    final_content = "".join(lines)
    final_content = re.sub(r"\n{3,}", "\n\n", final_content)
    if not final_content.endswith("\n"):
        final_content += "\n"

    with open(filepath, "w", encoding="utf-8") as f:
        f.write(final_content)

files = sorted(glob.glob(os.path.join(docs_dir, "*.md")))
for f in files:
    fix_file(f)

print("Finished processing all documentation files.")
