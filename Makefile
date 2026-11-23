.PHONY: help docs-serve docs-build docs-clean test-c23 check-links lint-md

CC ?= clang
CFLAGS ?= -std=c23 -Wall -Wextra -Werror -Wpedantic -O2
ZENSICAL := $(if $(wildcard .venv/bin/zensical),.venv/bin/zensical,uvx zensical)
PYTHON := $(if $(wildcard .venv/bin/python),.venv/bin/python,python3)

help:
	@echo "C Language Mastery & Complete C23 Reference — Commands"
	@echo "  make docs-serve   Serve Zensical docs locally with live reload (http://127.0.0.1:8000)"
	@echo "  make docs-build   Build static site into ./site"
	@echo "  make docs-clean   Remove generated ./site and ./.cache directories"
	@echo "  make test-c23     Compile and run all verified C23 reference examples"
	@echo "  make check-links  Validate internal Markdown links across docs/"
	@echo "  make lint-md      Run markdownlint across all Markdown files"

docs-serve:
	$(ZENSICAL) serve

docs-build:
	$(ZENSICAL) build --clean

docs-clean:
	rm -rf site .cache

test-c23:
	@set -e; \
	mkdir -p /tmp/c_lang_c23_bin; \
	for src in examples/c23/*.c; do \
		out="/tmp/c_lang_c23_bin/$$(basename "$$src" .c)"; \
		echo "[CC] $$src"; \
		$(CC) $(CFLAGS) "$$src" -lm -o "$$out"; \
		echo "[RUN] $$out"; \
		"$$out"; \
	done; \
	rm -rf /tmp/c_lang_c23_bin

check-links:
	$(PYTHON) scripts/check_docs.py

lint-md:
	npx --yes markdownlint-cli "**/*.md" --ignore node_modules --ignore site --ignore .venv
